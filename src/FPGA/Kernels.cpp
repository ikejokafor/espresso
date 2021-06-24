#include "Kernels.hpp"
using namespace std;


Kernels::Kernels(FPGA_hndl* fpga_hndl, int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, float* data) : Accel_Payload()
{
    m_fpga_hndl 		= fpga_hndl;
	m_numKernels 		= numKernels;
	m_kernelDepth 		= kernelDepth;
	m_numKernelRows 	= numKernelRows;
	m_numKernelCols 	= numKernelCols;
	m_cpu_data.resize(numKernels);
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
	for (int i = 0; i < numKernels; i++)
	{
		m_cpu_data[i].resize(kernelDepth);
		for (int j = 0; j < kernelDepth; j++)
		{
			int size = numKernelRows * numKernelCols * sizeof(float);
			m_cpu_data[i][j] = (float*)malloc(size);
			int krnl_step = kernelDepth * numKernelRows * numKernelCols;
			int dpth_idx = j * (numKernelRows * numKernelCols);
			int idx = index2D(krnl_step, i, dpth_idx);
			float* krnlWndw = &data[idx];
			int cpySize = numKernelRows * numKernelCols * sizeof(float);
			memcpy((void*)m_cpu_data[i][j], (void*)krnlWndw, cpySize);
		}
	}
}


Kernels::Kernels(FPGA_hndl* fpga_hndl, int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
	m_kernelDepth   = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
    m_cpu_data      = data;
    m_cpu_data.resize(numKernels);
    for (int i = 0; i < numKernels; i++)
	{
		m_cpu_data[i].resize(kernelDepth);
		for (int j = 0; j < kernelDepth; j++)
		{
			int size = numKernelRows * numKernelCols * sizeof(float);
			m_cpu_data[i][j] = (float*)malloc(size);
			int cpySize = numKernelRows * numKernelCols * sizeof(float);
			memcpy((void*)m_cpu_data[i][j], (void*)data[i][j], cpySize);
		}
	}
}


Kernels::~Kernels()
{
	for (int i = 0; i < m_numKernels; i++)
	{
		m_cpu_data[i].resize(m_kernelDepth);
		for (int j = 0; j < m_kernelDepth; j++)
		{
			free(m_cpu_data[i][j]);
		}
	}
#ifdef ALPHA_DATA
	_hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);  
#else
	SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);   
#endif
}


void Kernels::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                    = reinterpret_cast<_hndl*>(m_fpga_hndl);
	m_size                          = m_numKernels * m_kernelDepth * m_numKernelRows * m_numKernelCols * sizeof(fixedPoint_t);
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

	for(int n = 0; n < m_numKernels; n++) 
    {
        for(int d = 0; d < m_kernelDepth; d++)
        {
            for(int r = 0; r < m_numKernelRows; r++)
            {
                for(int c = 0; c < m_numKernelCols; c++)
                {
                    int rIdx = index4D(m_numKernelRows, m_numKernelCols, m_kernelDepth, n, r, c, d);
                    int cIdx = index2D(m_numKernelCols, r, c);
                    rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[n][d][cIdx]);    // FIXME: remove hardcoding, and pack values
                }
            }
        }
    }
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	m_size                          = 1024 * QUAD_DPTH_SIMD * m_numKernelRows * m_numKernelCols * sizeof(float); // FIXME, hardcoding
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    float* rmt_data                 = (float*)m_buffer;

	for(int n = 0; n < m_numKernels; n++) 
    {
        for(int d = 0; d < m_kernelDepth; d++)
        {
            for(int r = 0; r < m_numKernelRows; r++)
            {
                for(int c = 0; c < m_numKernelCols; c++)
                {
                    int rIdx = index4D(QUAD_DPTH_SIMD, m_numKernelRows, m_numKernelCols, n, d, r, c);
                    int cIdx = index2D(m_numKernelCols, r, c);
                    rmt_data[rIdx] = m_cpu_data[n][d][cIdx];
                }
            }
        }
    }
    
    FILE *fd = fopen("./kernels_fpga.txt", "w");
    for(int n = 0; n < m_numKernels; n++) 
    {
        for(int d = 0; d < m_kernelDepth; d++)
        {
            for(int r = 0; r < m_numKernelRows; r++)
            {
                for(int c = 0; c < m_numKernelCols; c++)
                {
                    int idx = index4D(QUAD_DPTH_SIMD, m_numKernelRows, m_numKernelCols, n, d, r, c);
                    fprintf(fd, "%f ", rmt_data[idx]);
                }
                fprintf(fd, "\n");
            }
            fprintf(fd, "\n\n\n");
        }
    }
    fclose(fd);
#endif
}


void Kernels::deserialize()
{

}


Kernels* Kernels::GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize)
{
	krnl_data_t krnl_data;
	krnl_data.resize(numKrnl);
	int krnl_ofst = krnlBgn;
	for (int i = 0; i < numKrnl; i++, krnl_ofst++)
	{
		krnl_data[i].resize(depthSize);
		int depth_ofst = depthBgn;
		for (int j = 0; j < depthSize; j++, depth_ofst++)
		{
			krnl_data[i][j] = m_cpu_data[krnl_ofst][depth_ofst];
		}
	}
	return new Kernels(m_fpga_hndl, numKrnl, depthSize, m_numKernelRows, m_numKernelCols, krnl_data);
}

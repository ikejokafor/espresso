#include "Kernels.hpp"
using namespace std;


Kernels::Kernels(FPGA_hndl* fpga_hndl, int numKernels, int depth, int rows, int cols, float* data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
	m_depth 		= depth;
	m_rows 	        = rows;
	m_cols 	        = cols;
	m_cpu_data.resize(numKernels);
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
	for (int i = 0; i < numKernels; i++)
	{
		m_cpu_data[i].resize(m_depth);
		for (int j = 0; j < m_depth; j++)
		{
			int size = m_rows * m_cols * sizeof(float);
			m_cpu_data[i][j] = (float*)malloc(size);
			int krnl_step = m_depth * m_rows * m_cols;
			int dpth_idx = j * (m_rows * m_cols);
			int idx = index2D(krnl_step, i, dpth_idx);
			float* krnlWndw = &data[idx];
			memcpy((void*)m_cpu_data[i][j], (void*)krnlWndw, size);
		}
	}
}


Kernels::Kernels(FPGA_hndl* fpga_hndl, int numKernels, int depth, int rows, int cols, krnl_data_t& data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
	m_depth         = depth;
	m_rows          = rows;
	m_cols          = cols;
    m_cpu_data      = data;
}


Kernels::~Kernels()
{
	for (int i = 0; i < m_numKernels; i++)
	{
		m_cpu_data[i].resize(m_depth);
		for (int j = 0; j < m_depth; j++)
		{
			free(m_cpu_data[i][j]);
		}
	}
#ifdef ALPHA_DATA
	_hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this); 
#endif
}


void Kernels::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                    = reinterpret_cast<_hndl*>(m_fpga_hndl);
	m_size                          = m_numKernels * m_depth * m_rows * m_cols * sizeof(fixedPoint_t);
    string ks = (m_numKernelCols == 1) ? "1" : "3";
    printf("[ESPRESSO]: Allocating Space for %sx%s Kernels\n", ks.c_str(), ks.c_str());    
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

	for(int n = 0; n < m_numKernels; n++) 
    {
        for(int d = 0; d < m_depth; d++)
        {
            for(int r = 0; r < m_rows; r++)
            {
                for(int c = 0; c < m_cols; c++)
                {
                    int rIdx = index4D(m_rows, m_cols, m_depth, n, r, c, d);
                    int cIdx = index2D(m_cols, r, c);
                    rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[n][d][cIdx]);    // FIXME: remove hardcoding, and pack values
                }
            }
        }
    }
#else
    m_size = m_numKernels * m_depth * m_rows * m_cols * PIXEL_SIZE;
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
            int size = m_rows * m_cols * sizeof(float);
			krnl_data[i][j] = (float*)malloc(size);
			memcpy(krnl_data[i][j], m_cpu_data[krnl_ofst][depth_ofst], size);
		}
	}
	return new Kernels(m_fpga_hndl, numKrnl, depthSize, m_rows, m_cols, krnl_data);
}

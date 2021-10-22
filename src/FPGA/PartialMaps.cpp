#include "PartialMaps.hpp"
using namespace std;


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data)
{
    m_fpga_hndl         = fpga_hndl;
	m_depth             = depth;
	m_rows              = rows;
	m_cols              = cols;
    m_cpu_data          = new float[depth * rows * cols];
	memcpy(m_cpu_data, data, sizeof(float) * depth * rows * cols);
    m_no_permute        = true;
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, InputMaps* inputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_depth             = inputMaps->m_depth;
	m_rows              = inputMaps->m_rows;
	m_cols              = inputMaps->m_cols; 
    m_cpu_data          = new float[m_depth * m_rows * m_cols];
	memcpy(m_cpu_data, inputMaps->m_cpu_data, sizeof(float) * m_depth * m_rows * m_cols);
    m_no_permute        = true;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_depth             = outputMaps->m_depth;
	m_rows              = outputMaps->m_rows;
	m_cols              = outputMaps->m_cols;
    m_cpu_data          = new float[m_depth * m_rows * m_cols];
    m_no_permute        = false;
}


PartialMaps::~PartialMaps()
{
    free(m_cpu_data);
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#endif
}


void PartialMaps::serialize()
{
    m_ftchAmt = AXI_sz_algn((m_depth * PM_LOW_WATERMARK_FACTOR * PIXEL_SIZE), AXI_MX_BT_SZ);
    m_ftchFctr = m_ftchAmt / PIXEL_SIZE;
    m_size = (int)ceil((float)(m_depth * m_rows * m_cols) / (float)m_ftchFctr) * m_ftchFctr * PIXEL_SIZE;
    m_vld_sz = m_depth * m_rows * m_cols * PIXEL_SIZE;
#ifdef ALPHA_DATA
    _hndl* _fpga_hndl  		    	= reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_depth * m_rows * m_cols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for Partial Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;
    if(m_no_permute) 
    {
        memcpy(m_buffer, m_cpu_data, m_size);
        return;
    }
	
    for(int r = 0; r < m_rows; r++)
    {
        for(int c = 0; c < m_cols; c++)
        {
            for(int d = 0; d < m_depth; d++)
            {
                int rIdx = index3D(m_cols, m_depth, r, c, d);
                int cIdx = index3D(m_rows, m_cols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding
            }
        }
    }
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_remAddress                    = (uint64_t)sysc_fpga_hndl->m_remAddrOfst;
    sysc_fpga_hndl->m_remAddrOfst   += m_size;
#endif

}


void PartialMaps::deserialize()
{

}

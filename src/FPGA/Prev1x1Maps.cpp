#include "Prev1x1Maps.hpp"
using namespace std;


Prev1x1Maps::Prev1x1Maps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl     = fpga_hndl;
	m_depth         = outputMaps->m_depth;
	m_rows          = outputMaps->m_rows;
	m_cols		    = outputMaps->m_cols;
    m_cpu_data      = new float[m_depth * m_rows * m_cols];
	memcpy(m_cpu_data, outputMaps->m_cpu_data, sizeof(float) * m_depth * m_rows * m_cols);
}


Prev1x1Maps::~Prev1x1Maps()
{
    free(m_cpu_data);
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#endif
}


void Prev1x1Maps::serialize()
{
    m_ftchAmt = AXI_sz_algn((m_depth * PV_LOW_WATERMARK_FACTOR * PIXEL_SIZE), AXI_MX_BT_SZ);
    m_ftchFctr = m_ftchAmt / PIXEL_SIZE;
    m_size = (int)ceil((float)(m_depth * m_rows * m_cols) / (float)m_ftchFctr) * m_ftchFctr * PIXEL_SIZE;
    m_vld_sz = m_depth * m_rows * m_cols * PIXEL_SIZE;
#ifdef ALPHA_DATA 
    _hndl* _hndl  					= reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_depth * m_rows * m_cols * sizeof(fixedPoint_t);
    printf("[ESPRESSO]: Allocating Space for Previous Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    memcpy(m_buffer, m_prevOutdata, m_size);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_remAddress                    = (uint64_t)sysc_fpga_hndl->m_remAddrOfst;
    sysc_fpga_hndl->m_remAddrOfst   += m_size;
#endif
}


void Prev1x1Maps::deserialize()
{

}

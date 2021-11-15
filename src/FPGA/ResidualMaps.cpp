#include "ResidualMaps.hpp"
using namespace std;


ResidualMaps::ResidualMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data) : Accel_Payload()
{
	m_fpga_hndl             = fpga_hndl;
	m_depth                 = depth;
	m_rows                  = rows;
	m_cols                  = cols;
    m_cpu_data              = new float[m_depth * m_rows * m_cols];
    memcpy(m_cpu_data, data, sizeof(float) * m_depth * m_rows * m_cols);
	m_buffer				= NULL;
	m_size              	= 0;
	m_remAddress        	= -1;
}


ResidualMaps::ResidualMaps(ResidualMaps* residualMaps)
{
	m_fpga_hndl             = residualMaps->m_fpga_hndl;
	m_depth                 = residualMaps->m_depth;
	m_rows                  = residualMaps->m_rows;
	m_cols                  = residualMaps->m_cols;
    m_cpu_data              = new float[m_depth * m_rows * m_cols];
    memcpy(m_cpu_data, residualMaps->m_cpu_data, sizeof(float) * m_depth * m_rows * m_cols);
	m_buffer				= NULL;
	m_size              	= 0;
	m_remAddress        	= -1;
}


ResidualMaps::~ResidualMaps()
{
    free(m_cpu_data);
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#endif
}


void ResidualMaps::serialize()
{
    m_ftchAmt = AXI_sz_algn((m_depth * RM_LOW_WATERMARK_FACTOR * PIXEL_SIZE), AXI_MX_BT_SZ);
    m_ftchFctr = m_ftchAmt / PIXEL_SIZE;
    m_size = (int)ceil((float)(m_depth * m_rows * m_cols) / (float)m_ftchFctr) * m_ftchFctr * PIXEL_SIZE;
    m_vld_sz = m_depth * m_rows * m_cols * PIXEL_SIZE;
#ifdef ALPHA_DATA
    _hndl*  _hndl                   = reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_depth * m_rows * m_cols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for Residual Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

    for(int r = 0; r < m_rows; r++)
    {
        for(int c = 0; c < m_cols; c++)
        {
            for(int d = 0; d < m_depth; d++)
            {
                int rIdx = index3D(m_cols, m_depth, r, c, d);
                int cIdx = index3D(m_rows, m_cols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding, and pack values
            }
        }
    }
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);  
    m_remAddress                    = (uint64_t)sysc_fpga_hndl->m_remAddrOfst;
    sysc_fpga_hndl->m_remAddrOfst   += m_size;
#endif

}


void ResidualMaps::deserialize()
{

}


ResidualMaps* ResidualMaps::GetVolume(int depthBgn, int depthSize)
{
    float* ptr = (float*)(m_cpu_data + (depthBgn * m_rows * m_cols));
	return new ResidualMaps(m_fpga_hndl, depthSize, m_rows, m_cols, ptr);
}

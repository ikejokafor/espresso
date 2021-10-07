#include "InputMaps.hpp"
using namespace std;


InputMaps::InputMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data) : Accel_Payload()
{
    m_fpga_hndl = fpga_hndl;
	m_depth     = depth;
	m_rows      = rows;
	m_cols      = cols;
	m_cpu_data  = new float[depth * rows * cols];
    memcpy(m_cpu_data, data, sizeof(float) * depth * rows * cols);
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


InputMaps::~InputMaps()
{
    free(m_cpu_data);    
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#endif
}


void InputMaps::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                = reinterpret_cast< hndl*>(m_fpga_hndl);
    m_size                      = m_depth * m_rows * m_cols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for InputMaps\n");
    m_buffer                    = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data      = (fixedPoint_t*)m_buffer;

	for(int r = 0; r < m_rows; r++)
	{
		for(int c = 0; c < m_cols; c++)
		{
			for(int d = 0; d < m_depth; d++)
			{
                int rIdx = index3D(m_cols, m_depth, r, c, d);
                int cIdx = index3D(m_rows, m_cols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding and pack values
            }
        }
    }
#else
    m_size = AXI_ceil((m_depth * m_rows * m_cols * PIXEL_SIZE), AXI_MX_BT_SZ);
#endif
}


void InputMaps::deserialize()
{

}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	float* ptr = (float*)(m_cpu_data + (depthBgn * m_rows * m_cols));
	return new InputMaps(m_fpga_hndl, depthSize, m_rows, m_cols, ptr);
}

#include "InputMaps.hpp"
using namespace std;


InputMaps::InputMaps(FPGA_hndl* fpga_hndl, int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_inputMapDepth     = inputMapDepth;
	m_numInputMapRows   = numInputMapRows;
	m_numInputMapCols   = numInputMapCols;
	m_cpu_data          = new float[inputMapDepth * numInputMapRows * numInputMapCols];
    memcpy(m_cpu_data, data, sizeof(float) * inputMapDepth * numInputMapRows * numInputMapCols);
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
    _hndl* _hndl                    = reinterpret_cast< hndl*>(m_fpga_hndl);
    m_size                          = m_inputMapDepth * m_numInputMapRows * m_numInputMapCols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for InputMaps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

	for(int r = 0; r < m_numInputMapRows; r++)
	{
		for(int c = 0; c < m_numInputMapCols; c++)
		{
			for(int d = 0; d < m_inputMapDepth; d++)
			{
                int rIdx = index3D(m_numInputMapCols, m_inputMapDepth, r, c, d);
                int cIdx = index3D(m_numInputMapRows, m_numInputMapCols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding and pack values
            }
        }
    }
#endif
}


void InputMaps::deserialize()
{

}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	float* ptr = (float*)(m_cpu_data + (depthBgn * m_numInputMapRows * m_numInputMapCols));
	return new InputMaps(m_fpga_hndl, depthSize, m_numInputMapRows, m_numInputMapCols, ptr);
}

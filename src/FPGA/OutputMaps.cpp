#include "OutputMaps.hpp"


OutputMaps::OutputMaps(FPGA_hndl* fpga_hndl, int outputMapDepth, int numOutputMapRows, int numOutputMapCols) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_outputMapDepth    = outputMapDepth;
	m_numOutputMapRows  = numOutputMapRows;
	m_numOutputMapCols  = numOutputMapCols;
    m_cpu_data          = new float[outputMapDepth * numOutputMapRows * numOutputMapCols];
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


OutputMaps::~OutputMaps()
{
    free(m_cpu_data);
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#endif
}


void OutputMaps::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                    = reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_outputMapDepth * m_numOutputMapRows * m_numOutputMapCols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for Output Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
#endif
}

void OutputMaps::deserialize()
{
#ifdef ALPHA_DATA

#else

#endif
}

#include "OutputMaps.hpp"


OutputMaps::OutputMaps(FPGA_hndl* fpga_hndl, int outputMapDepth, int numOutputMapRows, int numOutputMapCols) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_outputMapDepth    = outputMapDepth;
	m_numOutputMapRows  = numOutputMapRows;
	m_numOutputMapCols  = numOutputMapCols;
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


OutputMaps::~OutputMaps()
{
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);    
#endif
}


void OutputMaps::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                    = reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_numOutputMapRows * m_numOutputMapRows * m_numOutputMapCols * PIXEL_SIZE;
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = (uint64_t)1024 * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * (uint64_t)sizeof(float);  // FIXME, hardcoding
    printf("[ESPRESSO]: Allocating Space for Output Maps\n");
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
#endif
}

void OutputMaps::deserialize()
{
#ifdef ALPHA_DATA

#else

#endif
}

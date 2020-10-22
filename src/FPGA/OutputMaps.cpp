#include "OutputMaps.hpp"


OutputMaps::OutputMaps(FPGA_hndl* fpga_hndl, int outputMapDepth, int numOutputMapRows, int numOutputMapCols) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_outputMapDepth    = outputMapDepth;
	m_numOutputMapRows  = numOutputMapRows;
	m_numOutputMapCols  = numOutputMapCols; 
}


OutputMaps::~OutputMaps()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);
#else
    
#endif
}


void OutputMaps::serialize()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_numOutputMapRows * m_numOutputMapRows * m_numOutputMapCols * PIXEL_SIZE;
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
#else

#endif
}

void OutputMaps::deserialize()
{
#ifdef SYSTEMC

#else

#endif
}

#include "OutputMaps.hpp"


OutputMaps::OutputMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_depth             = depth;
	m_rows              = rows;
	m_cols              = cols;
    m_cpu_data          = new float[depth * rows * cols];
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
    _hndl* _hndl    = reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size          = m_depth * m_rows * m_cols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for Output Maps\n");
    m_buffer        = (void*)_hndl->allocate(this, m_size);
#else
    m_size = m_depth * m_rows * m_cols * PIXEL_SIZE;
#endif
}

void OutputMaps::deserialize()
{
#ifdef ALPHA_DATA

#else

#endif
}

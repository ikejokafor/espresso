#include "Prev1x1Maps.hpp"
using namespace std;



Prev1x1Maps::Prev1x1Maps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl             = fpga_hndl;
	m_prev1x1MapDepth       = outputMaps->m_outputMapDepth;
	m_numPrev1x1MapRows     = outputMaps->m_numOutputMapRows;
	m_numPrev1x1MapCols		= outputMaps->m_numOutputMapCols;
	m_prevOutdata			= outputMaps->m_buffer;
	m_buffer				= NULL;
	m_size              	= 0;
	m_remAddress        	= -1;
}


Prev1x1Maps::~Prev1x1Maps()
{
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);   
#endif
}


void Prev1x1Maps::serialize()
{
#ifdef ALPHA_DATA 
    _hndl* _hndl  					= reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_prev1x1MapDepth * m_numPrev1x1MapRows * m_numPrev1x1MapCols * sizeof(fixedPoint_t);
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    memcpy(m_buffer, m_prevOutdata, m_size);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = QUAD_DPTH_SIMD * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * sizeof(fixedPoint_t);
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    memcpy(m_buffer, m_prevOutdata, m_size);	
#endif
}


void Prev1x1Maps::deserialize()
{

}

#include "Prev1x1Maps.hpp"
using namespace std;



Prev1x1Maps::Prev1x1Maps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl             = fpga_hndl;
	m_prev1x1MapDepth       = outputMaps->m_outputMapDepth;
	m_numPrev1x1MapRows     = outputMaps->m_numOutputMapRows;
	m_numPrev1x1MapCols		= outputMaps->m_numOutputMapCols;
    m_cpu_data              = new float[m_prev1x1MapDepth * m_numPrev1x1MapRows * m_numPrev1x1MapCols];
	memcpy(m_cpu_data, outputMaps->m_cpu_data, sizeof(float) * m_prev1x1MapDepth * m_numPrev1x1MapRows * m_numPrev1x1MapCols);
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
#ifdef ALPHA_DATA 
    _hndl* _hndl  					= reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_prev1x1MapDepth * m_numPrev1x1MapRows * m_numPrev1x1MapCols * sizeof(fixedPoint_t);
    printf("[ESPRESSO]: Allocating Space for Previous Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    memcpy(m_buffer, m_prevOutdata, m_size);
#endif
}


void Prev1x1Maps::deserialize()
{

}

#include "Prev1x1Maps.hpp"
using namespace std;



Prev1x1Maps::Prev1x1Maps(OutputMaps* outputMaps)
{
	m_prev1x1MapDepth = outputMaps->m_outputMapDepth;
	m_numPrev1x1MapRows = outputMaps->m_numOutputMapRows;
	m_numPrev1x1MapCols = outputMaps->m_numOutputMapCols;
}


Prev1x1Maps::~Prev1x1Maps()
{
	deallocate();
}


uint64_t Prev1x1Maps::allocate(int size)
{

}


void Prev1x1Maps::deallocate()
{

}


void Prev1x1Maps::serialize()
{
	m_size = m_prev1x1MapDepth * m_numPrev1x1MapRows * m_numPrev1x1MapCols * PIXEL_SIZE;
}


void Prev1x1Maps::deserialize()
{

}

#include "OutputMaps.hpp"


OutputMaps::OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols) : Accel_Payload()
{
	m_outputMapDepth = outputMapDepth;
	m_numOutputMapRows = numOutputMapRows;
	m_numOutputMapCols = numOutputMapCols;
}


OutputMaps::~OutputMaps()
{
	deallocate();
}


uint64_t OutputMaps::allocate(int size)
{

}


void OutputMaps::deallocate()
{

}


void OutputMaps::serialize()
{
	m_size = m_outputMapDepth * m_numOutputMapRows * m_numOutputMapCols * PIXEL_SIZE;
}

void OutputMaps::deserialize()
{

}

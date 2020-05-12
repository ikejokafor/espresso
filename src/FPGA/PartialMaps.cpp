#include "PartialMaps.hpp"
using namespace std;


PartialMaps::PartialMaps(InputMaps* inputMaps)
{
	m_partialMapDepth = inputMaps->m_inputMapDepth;
	m_numPartialMapRows = inputMaps->m_numInputMapRows;
	m_numPartialMapCols = inputMaps->m_numInputMapCols;
}


PartialMaps::PartialMaps(OutputMaps* outputMaps)
{
	m_partialMapDepth = outputMaps->m_outputMapDepth;
	m_numPartialMapRows = outputMaps->m_numOutputMapRows;
	m_numPartialMapCols = outputMaps->m_numOutputMapCols;
}


PartialMaps::~PartialMaps()
{
	deallocate();
}


uint64_t PartialMaps::allocate(int size)
{

}


void PartialMaps::deallocate()
{

}


void PartialMaps::serialize()
{
	m_size = m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols * PIXEL_SIZE;
}


void PartialMaps::deserialize()
{

}

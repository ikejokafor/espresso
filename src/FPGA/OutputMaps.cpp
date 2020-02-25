#include "OutputMaps.hpp"


OutputMaps::OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols, fixedPoint_t* data)
{
	m_outputMapDepth = outputMapDepth;
	m_numOutputMapRows = numOutputMapRows;
	m_numOutputMapCols = numOutputMapCols;
	m_data = data;
}


OutputMaps::~OutputMaps()
{
	
}


OutputMaps* OutputMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numOutputMapRows * m_numOutputMapCols));
	return new OutputMaps((depthBgn + depthSize), m_numOutputMapRows, m_numOutputMapCols, ptr);
}
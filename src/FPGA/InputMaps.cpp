#include "InputMaps.hpp"


InputMaps::InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, fixedPoint_t* data)
{
	m_inputMapDepth = inputMapDepth;
	m_numInputMapRows = numInputMapRows;
	m_numInputMapCols = numInputMapCols;
	m_data = data;
}


InputMaps::~InputMaps()
{
	
}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numInputMapRows * m_numInputMapCols));
	return new InputMaps(depthSize, m_numInputMapRows, m_numInputMapCols, ptr);
}
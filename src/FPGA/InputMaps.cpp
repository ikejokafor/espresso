#include "InputMaps.hpp"


InputMaps::InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols)
{
	m_inputMapDepth = inputMapDepth;
	m_numInputMapRows = numInputMapRows;
	m_numInputMapCols = numInputMapCols;
}


InputMaps::~InputMaps()
{
	
}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthEnd)
{
	return new InputMaps((depthEnd - depthBgn) + 1, m_numInputMapRows, m_numInputMapCols);
}
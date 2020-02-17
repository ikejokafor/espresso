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


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	return new InputMaps((depthBgn + depthSize), m_numInputMapRows, m_numInputMapCols);
}
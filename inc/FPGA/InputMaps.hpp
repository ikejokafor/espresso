#pragma once


#include "fixedPoint.hpp"


class InputMaps
{
	public:
		InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, fixedPoint_t* data);
		~InputMaps();
		InputMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_inputMapDepth;
		int m_numInputMapRows;
		int m_numInputMapCols;
		fixedPoint_t* m_data;
};
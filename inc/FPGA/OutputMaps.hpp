#pragma once


#include "fixedPoint.hpp"


class OutputMaps
{
	public:
		OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols, fixedPoint_t* data);
		~OutputMaps();
		OutputMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_outputMapDepth;
		int m_numOutputMapRows;
		int m_numOutputMapCols;
		fixedPoint_t* m_data;
};
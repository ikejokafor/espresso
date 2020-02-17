#pragma once


class InputMaps
{
	public:
		InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols);
		~InputMaps();
		InputMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_inputMapDepth;
		int m_numInputMapRows;
		int m_numInputMapCols;
};
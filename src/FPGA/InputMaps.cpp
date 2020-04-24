#include "InputMaps.hpp"
using namespace std;


InputMaps::InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data) : Accel_Payload()
{
	m_inputMapDepth = inputMapDepth;
	m_numInputMapRows = numInputMapRows;
	m_numInputMapCols = numInputMapCols;
}


InputMaps::~InputMaps()
{
	deallocate();
}


uint64_t InputMaps::allocate(int size)
{

}


void InputMaps::deallocate()
{

}


void InputMaps::serialize()
{
	m_size = m_inputMapDepth * m_numInputMapRows * m_numInputMapCols * PIXEL_SIZE;
}

void InputMaps::deserialize()
{

}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	float* ptr;
#ifdef FPGA
	float* ptr = (float*)(m_data + (depthBgn * m_numInputMapRows * m_numInputMapCols));
#endif
	return new InputMaps(depthSize, m_numInputMapRows, m_numInputMapCols, ptr);
}

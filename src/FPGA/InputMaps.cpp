#include "InputMaps.hpp"
using namespace std;


InputMaps::InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, fixedPoint_t* data) : Accel_Payload()
{
	m_inputMapDepth = inputMapDepth;
	m_numInputMapRows = numInputMapRows;
	m_numInputMapCols = numInputMapCols;
	int size = inputMapDepth * numInputMapRows * numInputMapCols * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


InputMaps::~InputMaps()
{
	deallocate();
}


uint64_t InputMaps::allocate(int size)
{
#ifdef SYSTEMC
	m_size = size;
	m_address = (uint64_t)malloc(size);
#else

#endif
	return m_address;
}


void InputMaps::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void InputMaps::serialize()
{
	m_size = m_inputMapDepth * m_numInputMapRows * m_numInputMapCols * PIXEL_SIZE;
#ifdef SYSTEMC

#else

#endif
}

void InputMaps::deserialize()
{
#ifdef SYSTEMC

#else

#endif
}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numInputMapRows * m_numInputMapCols));
	return new InputMaps(depthSize, m_numInputMapRows, m_numInputMapCols, ptr);
}

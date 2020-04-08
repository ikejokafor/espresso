#include "OutputMaps.hpp"


OutputMaps::OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols) : Accel_Payload()
{
	m_outputMapDepth = outputMapDepth;
	m_numOutputMapRows = numOutputMapRows;
	m_numOutputMapCols = numOutputMapCols;
	int size = outputMapDepth * numOutputMapRows * numOutputMapCols * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
}


OutputMaps::OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols, fixedPoint_t* data) : Accel_Payload()
{
	m_outputMapDepth = outputMapDepth;
	m_numOutputMapRows = numOutputMapRows;
	m_numOutputMapCols = numOutputMapCols;
	int size = outputMapDepth * numOutputMapRows * numOutputMapCols * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


OutputMaps::~OutputMaps()
{
	deallocate();
}


uint64_t OutputMaps::allocate(int size)
{
#ifdef SYSTEMC
	m_address = (uint64_t)malloc(size);
#else

#endif
	return m_address;
}


void OutputMaps::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void OutputMaps::serialize()
{
	m_size = m_outputMapDepth * m_numOutputMapRows * m_numOutputMapCols * PIXEL_SIZE;
#ifdef SYSTEMC

#else

#endif
}

void OutputMaps::deserialize()
{
#ifdef SYSTEMC

#else

#endif
}


OutputMaps* OutputMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numOutputMapRows * m_numOutputMapCols));
	return new OutputMaps(depthSize, m_numOutputMapRows, m_numOutputMapCols, ptr);
}
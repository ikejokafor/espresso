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
	m_size = size;
#ifdef SYSTEMC
	return (uint64_t)malloc(size);
#else

#endif
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
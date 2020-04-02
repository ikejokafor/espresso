#include "PartialMaps.hpp"
using namespace std;


PartialMaps::PartialMaps(int partialMapDepth, int numPartialMapRows, int numPartialMapCols, fixedPoint_t* data) : Accel_Payload()
{
	m_partialMapDepth = partialMapDepth;
	m_numPartialMapRows = numPartialMapRows;
	m_numPartialMapCols = numPartialMapCols;
	int size = partialMapDepth * numPartialMapRows * numPartialMapCols * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


PartialMaps::~PartialMaps()
{
	deallocate();
}


uint64_t PartialMaps::allocate(int size)
{
	m_size = size;
#ifdef SYSTEMC
	return (uint64_t)malloc(size);
#else

#endif
}


void PartialMaps::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void PartialMaps::serialize()
{
#ifdef SYSTEMC
	
#else

#endif
}

void PartialMaps::deserialize()
{
#ifdef SYSTEMC
	
#else

#endif
}


PartialMaps* PartialMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numPartialMapRows * m_numPartialMapCols));
	return new PartialMaps(depthSize, m_numPartialMapRows, m_numPartialMapCols, ptr);
}

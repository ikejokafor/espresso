#include "ResidualMaps.hpp"
using namespace std;


ResidualMaps::ResidualMaps(int residualMapDepth, int numResidualMapRows, int numResidualMapCols, fixedPoint_t* data) : Accel_Payload()
{
	m_residualMapDepth = residualMapDepth;
	m_numResidualMapRows = numResidualMapRows;
	m_numResidualMapCols = numResidualMapCols;
	int size = residualMapDepth * numResidualMapRows * numResidualMapCols * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


ResidualMaps::~ResidualMaps()
{
	deallocate();
}


uint64_t ResidualMaps::allocate(int size)
{
	m_size = size;
#ifdef SYSTEMC
	return (uint64_t)malloc(size);
#else

#endif
}


void ResidualMaps::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void ResidualMaps::serialize()
{
#ifdef SYSTEMC
	
#else

#endif
}

void ResidualMaps::deserialize()
{
#ifdef SYSTEMC
	
#else

#endif
}


void ResidualMaps::permuteData()
{
	
}


ResidualMaps* ResidualMaps::GetVolume(int depthBgn, int depthSize)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + (depthBgn * m_numResidualMapRows * m_numResidualMapCols));
	return new ResidualMaps(depthSize, m_numResidualMapRows, m_numResidualMapCols, ptr);
}

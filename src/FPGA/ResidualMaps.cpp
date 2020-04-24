#include "ResidualMaps.hpp"
using namespace std;


ResidualMaps::ResidualMaps(int residualMapDepth, int numResidualMapRows, int numResidualMapCols, float* data) : Accel_Payload()
{
	m_residualMapDepth = residualMapDepth;
	m_numResidualMapRows = numResidualMapRows;
	m_numResidualMapCols = numResidualMapCols;
#ifdef FPGA
	int size = residualMapDepth * numResidualMapRows * numResidualMapCols * sizeof(float);
	m_data = (float*)allocate(size);
	memcpy(m_data, data, size);
#endif
}


ResidualMaps::~ResidualMaps()
{
	deallocate();
}


uint64_t ResidualMaps::allocate(int size)
{

}


void ResidualMaps::deallocate()
{

}


void ResidualMaps::serialize()
{
	m_size = m_residualMapDepth * m_numResidualMapRows * m_numResidualMapCols * PIXEL_SIZE;
}

void ResidualMaps::deserialize()
{

}


void ResidualMaps::permuteData()
{

}


ResidualMaps* ResidualMaps::GetVolume(int depthBgn, int depthSize)
{
	float* ptr;
#ifdef FPGA
	ptr = (float*)(m_data + (depthBgn * m_numResidualMapRows * m_numResidualMapCols));
#endif
	return new ResidualMaps(depthSize, m_numResidualMapRows, m_numResidualMapCols, ptr);
}

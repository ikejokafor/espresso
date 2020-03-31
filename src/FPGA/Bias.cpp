#include "Bias.hpp"
using namespace std;


Bias::Bias(int numKernels, fixedPoint_t* data) : Accel_Payload()
{
	m_numKernels = numKernels;
	int size = m_numKernels * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


Bias::~Bias()
{
	deallocate();
}


uint64_t Bias::allocate(int size)
{
#ifdef SYSTEMC
	m_size = size;
	return (uint64_t)malloc(size);
#else

#endif
}


void Bias::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void Bias::serialize()
{
#ifdef SYSTEMC
	
#else

#endif
}

void Bias::deserialize()
{
#ifdef SYSTEMC
	
#else

#endif
}


Bias* Bias::GetVolume(int krnlBgn, int numKrnl)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + krnlBgn);
	return new Bias(numKrnl, ptr);
}

#include "KernelBias.hpp"
using namespace std;


KernelBias::KernelBias(int numKernels, fixedPoint_t* data) : Accel_Payload()
{
	m_numKernels = numKernels;
	int size = m_numKernels * sizeof(fixedPoint_t);
	m_data = (fixedPoint_t*)allocate(size);
	memcpy(m_data, data, size);
}


KernelBias::~KernelBias()
{
	deallocate();
}


uint64_t KernelBias::allocate(int size)
{
	m_size = size;	
#ifdef SYSTEMC
	m_address = (uint64_t)malloc(size);
#else

#endif
	return m_address;
}


void KernelBias::deallocate()
{
#ifdef SYSTEMC
	free(m_data);
#else

#endif
}


void KernelBias::serialize()
{
#ifdef SYSTEMC
	
#else

#endif
}


void KernelBias::deserialize()
{
#ifdef SYSTEMC
	
#else

#endif
}


KernelBias* KernelBias::GetVolume(int krnlBgn, int numKrnl)
{
	fixedPoint_t* ptr = (fixedPoint_t*)(m_data + krnlBgn);
	return new KernelBias(numKrnl, ptr);
}

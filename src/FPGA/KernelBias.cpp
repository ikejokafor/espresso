#include "KernelBias.hpp"
using namespace std;


KernelBias::KernelBias(int numKernels, float* data) : Accel_Payload()
{
	m_numKernels = numKernels;
}


KernelBias::~KernelBias()
{
	deallocate();
}


uint64_t KernelBias::allocate(int size)
{

}


void KernelBias::deallocate()
{

}


void KernelBias::serialize()
{
	m_size = m_numKernels * PIXEL_SIZE;
}


void KernelBias::deserialize()
{

}


KernelBias* KernelBias::GetVolume(int krnlBgn, int numKrnl)
{
	float* ptr;
#ifdef FPGA
	ptr = (float*)(m_data + krnlBgn);
#endif
	return new KernelBias(numKrnl, ptr);
}

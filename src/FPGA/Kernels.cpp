#include "Kernels.hpp"
using namespace std;


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, fixedPoint_t* data) : Accel_Payload()
{
	m_numKernels = numKernels;
	m_kernelDepth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
	m_data.resize(numKernels);
	m_data_sz.resize(numKernels);
	for (int i = 0; i < numKernels; i++)
	{
		m_data[i].resize(kernelDepth);
		for (int j = 0; j < kernelDepth; j++)
		{
			int size = numKernelRows * numKernelCols * sizeof(fixedPoint_t);
			m_data[i][j] = (fixedPoint_t*)allocate(size);
			int krnl_step = kernelDepth * numKernelRows * numKernelCols;
			int dpth_idx = j * (numKernelRows * numKernelCols);
			int idx = index2D(krnl_step, i, dpth_idx);
			fixedPoint_t* kernel = &data[idx];
			int cpySize = numKernelRows * numKernelCols * sizeof(fixedPoint_t);
			memcpy((void*)m_data[i][j], (void*)kernel, cpySize);
		}
	}
}


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data) : Accel_Payload()
{
	m_numKernels = numKernels;
	m_kernelDepth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
	m_data = data;
}


Kernels::~Kernels()
{
	deallocate();
}


uint64_t Kernels::allocate(int size)
{
#ifdef SYSTEMC
	m_address = (uint64_t)malloc(size);
#else

#endif
	return m_address;
}


void Kernels::deallocate()
{
	for (int i = 0; i < m_numKernels; i++)
	{
		for (int j = 0; j < m_kernelDepth; j++)
		{
#ifdef SYSTEMC
			int idx = index2D(m_kernelDepth, i, j);
			free(m_data[i][j]);
#else

#endif
		}
	}
}


void Kernels::serialize()
{
#ifdef SYSTEMC
	m_size = m_numKernels * m_kernelDepth * m_numKernelRows * m_numKernelCols * PIXEL_SIZE;
	m_address = -1;
#else

#endif
}


void Kernels::deserialize()
{
#ifdef SYSTEMC

#else

#endif
}


Kernels* Kernels::GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize)
{
	krnl_data_t krnl_data;
	krnl_data.resize(numKrnl);
	int krnl_ofst = krnlBgn;
	for (int i = 0; i < numKrnl; i++, krnl_ofst++)
	{
		krnl_data[i].resize(depthSize);
		int depth_ofst = depthBgn;
		for (int j = 0; j < depthSize; j++, depth_ofst++)
		{
			krnl_data[i][j] = m_data[krnl_ofst][depth_ofst];
		}
	}
	return new Kernels(numKrnl, depthSize, m_numKernelRows, m_numKernelCols, krnl_data);
}

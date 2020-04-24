#include "Kernels.hpp"
using namespace std;


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, float* data) : Accel_Payload()
{
	m_numKernels = numKernels;
	m_kernelDepth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
#ifdef FPGA
	m_data.resize(numKernels);
	for (int i = 0; i < numKernels; i++)
	{
		m_data[i].resize(kernelDepth);
		for (int j = 0; j < kernelDepth; j++)
		{
			int size = numKernelRows * numKernelCols * sizeof(float);
			m_data[i][j] = (float*)allocate(size);
			int krnl_step = kernelDepth * numKernelRows * numKernelCols;
			int dpth_idx = j * (numKernelRows * numKernelCols);
			int idx = index2D(krnl_step, i, dpth_idx);
			float* kernel = &data[idx];
			int cpySize = numKernelRows * numKernelCols * sizeof(float);
			memcpy((void*)m_data[i][j], (void*)kernel, cpySize);
		}
	}
#endif
}


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data) : Accel_Payload()
{
	m_numKernels = numKernels;
	m_kernelDepth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
}


Kernels::~Kernels()
{
	deallocate();
}


uint64_t Kernels::allocate(int size)
{

}


void Kernels::deallocate()
{
#ifdef FPGA
	for (int i = 0; i < m_numKernels; i++)
	{
		for (int j = 0; j < m_kernelDepth; j++)
		{

		}
	}
#endif
}


void Kernels::serialize()
{
	m_size = m_numKernels * m_kernelDepth * m_numKernelRows * m_numKernelCols * PIXEL_SIZE;
}


void Kernels::deserialize()
{

}


Kernels* Kernels::GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize)
{
	krnl_data_t krnl_data;
#ifdef FPGA
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
#endif
	return new Kernels(numKrnl, depthSize, m_numKernelRows, m_numKernelCols, krnl_data);
}

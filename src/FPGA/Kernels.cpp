#include "Kernels.hpp"
using namespace std;


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, fixedPoint_t* data)
{
	m_numKernels = numKernels;
	m_kernelDpeth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
	m_data.resize(numKernels);
	for (int i = 0; i < numKernels; i++)
	{
		m_data[i].resize(kernelDepth);
		for (int j = 0; j < kernelDepth; j++)
		{
			m_data[i][j] = new fixedPoint_t[numKernelRows * numKernelCols];
			int krnl_step = kernelDepth * numKernelRows * numKernelCols;
			int dpth_idx = j * (numKernelRows * numKernelCols);
			int idx = index2D(krnl_step, i, dpth_idx);
			fixedPoint_t* kernel = &data[idx];
			int cpySize = numKernelRows * numKernelCols * sizeof(fixedPoint_t);
			memcpy(kernel, (void*)data, cpySize);
		}
	}
}


Kernels::Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data)
{
	m_numKernels = numKernels;
	m_kernelDpeth = kernelDepth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
	m_data.resize(numKernels);
	m_data = data;
}


Kernels::~Kernels()
{

}


Kernels* Kernels::GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize)
{	
	krnl_data_t krnl_data;
	int krnl_ofst = krnlBgn;
	int depth_ofst = depthBgn;
	for (int i = 0; i < numKrnl; i++, krnl_ofst++)
	{
		for (int j = 0; j < depthSize; j++, depth_ofst++)
		{
			krnl_data[i][j] = m_data[krnl_ofst][depth_ofst];
		}
	}
	return new Kernels(numKrnl, depthSize, m_numKernelRows, m_numKernelCols, krnl_data);
}

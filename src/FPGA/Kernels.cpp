#include "Kernels.hpp"


Kernels::Kernels(int numKernels, int kernelDpeth, int numKernelRows, int numKernelCols)
{
	m_numKernels = numKernels;
	m_kernelDpeth = kernelDpeth;
	m_numKernelRows = numKernelRows;
	m_numKernelCols = numKernelCols;
}


Kernels::~Kernels()
{

}


Kernels* Kernels::GetVolume(int krnl_bgn, int krnl_end, int depthBgn, int depthEnd)
{
	return new Kernels((krnl_end - krnl_bgn) + 1, (depthEnd - depthBgn) + 1, m_numKernelRows, m_numKernelCols);
}

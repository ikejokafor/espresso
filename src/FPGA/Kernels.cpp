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


Kernels* Kernels::GetVolume(int krnl_bgn, int numKrnl, int depthBgn, int depthSize)
{
	return new Kernels((krnl_bgn + numKrnl), (depthBgn + depthSize), m_numKernelRows, m_numKernelCols);
}

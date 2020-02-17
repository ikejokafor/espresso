#pragma once


class Kernels
{
	public:
		Kernels(int numKernels, int kernelDpeth, int numKernelRows, int numKernelCols);
		~Kernels();
		Kernels* GetVolume(int krnl_bgn, int numKrnl, int depthBgn, int depthSize);
	
		int m_numKernels;
		int m_kernelDpeth;
		int m_numKernelRows;
		int m_numKernelCols;
};
#pragma once

#include <vector>
#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"


class Kernels
{
	typedef std::vector<std::vector<fixedPoint_t*>> krnl_data_t;
	
	public:
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, fixedPoint_t* data);
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data);
		~Kernels();
		Kernels* GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize);
	
		int m_numKernels;
		int m_kernelDpeth;
		int m_numKernelRows;
		int m_numKernelCols;
		krnl_data_t m_data;
};
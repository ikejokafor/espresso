#pragma once


#include <vector>
#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class Kernels : public Accel_Payload
{
	typedef std::vector<std::vector<float*>> krnl_data_t;

	public:
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, float* data);
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data);
		~Kernels();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		Kernels* GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize);

		int m_numKernels;
		int m_kernelDepth;
		int m_numKernelRows;
		int m_numKernelCols;
		krnl_data_t m_data;
};

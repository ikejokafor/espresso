#pragma once


#include <vector>
#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class Kernels : public Accel_Payload
{
	typedef std::vector<std::vector<fixedPoint_t*>> krnl_data_t;
	typedef std::vector<int> shm_fd_t;

	
	public:
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, fixedPoint_t* data);
		Kernels(int numKernels, int kernelDepth, int numKernelRows, int numKernelCols, krnl_data_t data);
		~Kernels();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		Kernels* GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize);
		Kernels* GetVolume(int krnlBgn, int numKrnl);
	
		int m_numKernels;
		int m_kernelDepth;
		int m_numKernelRows;
		int m_numKernelCols;
		krnl_data_t m_data;
#ifdef SYSTEMC
		shm_fd_t m_shm_fd;
		int m_num_shm;
#else

#endif
};

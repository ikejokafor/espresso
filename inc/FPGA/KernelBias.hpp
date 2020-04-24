#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class KernelBias : public Accel_Payload
{
	public:
		KernelBias(int numKernels, float* data);
		~KernelBias();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		KernelBias* GetVolume(int krnlBgn, int numKrnl);

		int m_numKernels;
		float* m_data;
};

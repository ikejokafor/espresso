#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif

class KernelBias : public Accel_Payload
{
	public:
		KernelBias(FPGA_hndl* fpga_hndl, int numKernels, float* data);
		~KernelBias();
		void serialize();
        void deserialize();
		KernelBias* GetVolume(int krnlBgn, int numKrnl);

		int m_numKernels;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

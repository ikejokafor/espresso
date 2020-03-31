#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class Bias : public Accel_Payload
{
	public:
		Bias(int numKernels, fixedPoint_t* data);
		~Bias();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		Bias* GetVolume(int krnlBgn, int numKrnl);
	
		int m_numKernels;
		fixedPoint_t* m_data;
#ifdef SYSTEMC
		int m_shm_fd;
#else

#endif
};

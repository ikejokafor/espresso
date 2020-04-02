#pragma once


// System Includes
#include <stdint.h>
#include <string>
#include "FPGA_shim.hpp"
#include "util.hpp"
#include "espresso_FPGA_common.hpp"


class PixelSeqCfg : public Accel_Payload
{
	public:
		PixelSeqCfg(int stride);
		~PixelSeqCfg();
		uint64_t allocate(int size);
		void deallocate();
		void createCfg();
		void serialize();
        void deserialize();
		void stride1_config();
		void stride2_config();
	
		int m_stride;
};

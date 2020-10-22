#pragma once


// System Includes
#include <stdint.h>
#include <string>
#include "FPGA_shim.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif
#include "util.hpp"
#include "espresso_FPGA_common.hpp"


class PixelSeqCfg : public Accel_Payload
{
	public:
		PixelSeqCfg(FPGA_hndl* fpga_hndl, int stride);
		~PixelSeqCfg();
		void createCfg();
		void serialize();
        void deserialize();
		void stride1_config();
		void stride2_config();
	
		int m_stride;
		uint16_t* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

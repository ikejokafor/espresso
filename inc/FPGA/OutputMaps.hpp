#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif



class OutputMaps : public Accel_Payload
{
	public:
		OutputMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols);
		~OutputMaps();
		void serialize();
        void deserialize();

		int m_depth;
		int m_rows;
		int m_cols;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

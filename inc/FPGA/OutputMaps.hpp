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
		OutputMaps(FPGA_hndl* fpga_hndl, int outputMapDepth, int numOutputMapRows, int numOutputMapCols);
		~OutputMaps();
		void serialize();
        void deserialize();

		int m_outputMapDepth;
		int m_numOutputMapRows;
		int m_numOutputMapCols;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

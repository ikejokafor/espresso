#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif
#include "espresso_FPGA_common.hpp"


class InputMaps : public Accel_Payload
{
	public:
		InputMaps(FPGA_hndl* fpga_hndl, int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data);
		~InputMaps();
		void serialize();
        void deserialize();
		InputMaps* GetVolume(int depthBgn, int depthSize);

		int m_inputMapDepth;
		int m_numInputMapRows;
		int m_numInputMapCols;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

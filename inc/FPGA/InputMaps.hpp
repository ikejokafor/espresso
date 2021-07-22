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
		InputMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data);
		~InputMaps();
		void serialize();
        void deserialize();
		InputMaps* GetVolume(int depthBgn, int depthSize);

		int m_depth;
		int m_rows;
		int m_cols;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

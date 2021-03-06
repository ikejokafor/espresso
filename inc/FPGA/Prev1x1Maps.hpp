#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "InputMaps.hpp"
#include "OutputMaps.hpp"
#include "espresso_FPGA_common.hpp"


class Prev1x1Maps : public Accel_Payload
{
	public:
        Prev1x1Maps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps);
		~Prev1x1Maps();
		void serialize();
        void deserialize();


		int m_depth;
		int m_rows;
		int m_cols;
        int m_ftchAmt;
        int m_ftchFctr;
        int m_vld_sz;
        float* m_cpu_data;
        bool ref;
        FPGA_hndl* m_fpga_hndl;
};

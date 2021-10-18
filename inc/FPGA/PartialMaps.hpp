#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#include "InputMaps.hpp"
#include "OutputMaps.hpp"
#include "espresso_FPGA_common.hpp"


class PartialMaps : public Accel_Payload
{
	public:
        PartialMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data);
        PartialMaps(FPGA_hndl* fpga_hndl, InputMaps* inputMaps);
        PartialMaps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps);
		~PartialMaps();
		void serialize();
        void deserialize();


		int m_depth;
		int m_rows;
		int m_cols;
		bool m_no_permute;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

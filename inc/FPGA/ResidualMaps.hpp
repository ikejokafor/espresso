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


class ResidualMaps : public Accel_Payload
{
	public:
		ResidualMaps(FPGA_hndl* fpga_hndl, int depth, int rows, int cols, float* data);
		ResidualMaps(ResidualMaps* residualMaps);
        ~ResidualMaps();
        void serialize();
        void deserialize();
		void permuteData();
		ResidualMaps* GetVolume(int depthBgn, int depthSize);

		int m_depth;
		int m_rows;
		int m_cols;
        int m_ftchAmt;
        int m_ftchFctr;
        int m_vld_sz;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

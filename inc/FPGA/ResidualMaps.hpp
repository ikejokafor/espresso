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
		ResidualMaps(FPGA_hndl* fpga_hndl, int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data);
		~ResidualMaps();
        void serialize();
        void deserialize();
		void permuteData();
		ResidualMaps* GetVolume(int depthBgn, int depthSize);

		int m_residualMapDepth;
		int m_numResidualMapRows;
		int m_numResidualMapCols;
		float* m_cpu_data;
        FPGA_hndl* m_fpga_hndl;
};

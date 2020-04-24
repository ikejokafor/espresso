#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class ResidualMaps : public Accel_Payload
{
	public:
		ResidualMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data);
		~ResidualMaps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		void permuteData();
		ResidualMaps* GetVolume(int depthBgn, int depthSize);

		int m_residualMapDepth;
		int m_numResidualMapRows;
		int m_numResidualMapCols;
		float* m_data;
};

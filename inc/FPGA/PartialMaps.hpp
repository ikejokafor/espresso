#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "InputMaps.hpp"
#include "OutputMaps.hpp"
#include "espresso_FPGA_common.hpp"


class PartialMaps : public Accel_Payload
{
	public:
        PartialMaps(InputMaps* partialMaps);
        PartialMaps(OutputMaps* outputMaps);
		~PartialMaps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();


		int m_partialMapDepth;
		int m_numPartialMapRows;
		int m_numPartialMapCols;
		float* m_data;
};

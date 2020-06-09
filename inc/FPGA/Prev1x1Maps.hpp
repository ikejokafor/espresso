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
        Prev1x1Maps(OutputMaps* outputMaps);
		~Prev1x1Maps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();


		int m_prev1x1MapDepth;
		int m_numPrev1x1MapRows;
		int m_numPrev1x1MapCols;
		float* m_data;
};

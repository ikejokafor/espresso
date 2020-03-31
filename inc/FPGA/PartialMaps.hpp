#pragma once


#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class PartialMaps : public Accel_Payload
{
	public:
		PartialMaps(int partialMapDepth, int numPartialMapRows, int numPartialMapCols, fixedPoint_t* data);
		~PartialMaps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		PartialMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_partialMapDepth;
		int m_numPartialMapRows;
		int m_numPartialMapCols;
		fixedPoint_t* m_data;
#ifdef SYSTEMC
		int m_shm_fd;
#else

#endif
};

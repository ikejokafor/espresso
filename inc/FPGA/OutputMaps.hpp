#pragma once


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "espresso_FPGA_common.hpp"


class OutputMaps : public Accel_Payload
{
	public:
		OutputMaps(int outputMapDepth, int numOutputMapRows, int numOutputMapCols, fixedPoint_t* data);
		~OutputMaps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		OutputMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_outputMapDepth;
		int m_numOutputMapRows;
		int m_numOutputMapCols;
		fixedPoint_t* m_data;
#ifdef SYSTEMC
		int m_shm_fd;
#else

#endif
};
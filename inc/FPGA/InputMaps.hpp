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


class InputMaps : public Accel_Payload
{
	public:
		InputMaps(int inputMapDepth, int numInputMapRows, int numInputMapCols, fixedPoint_t* data);
		~InputMaps();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		InputMaps* GetVolume(int depthBgn, int depthSize);
	
		int m_inputMapDepth;
		int m_numInputMapRows;
		int m_numInputMapCols;
		fixedPoint_t* m_data;
#ifdef SYSTEMC
		int m_shm_fd;
#else

#endif
};
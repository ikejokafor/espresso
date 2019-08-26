#pragma once

#define NUM_KERNEL_3x3_VAL			10
#define QUAD_MAX_NUM_KRNL			64
#define QUAD_MAX_KRNL_DEPTH			8

#include <stdint.h>
#include <iostream>
#include <fstream>

#include "fixedPoint.hpp"
#include "util.hpp"

class kernel_group_config
{
	public:
		kernel_group_config(int kernels, int channels, int numfilValues, int fxPtLen, int numFracBits);
		~kernel_group_config();

		void set_data(int k, int c, int v, fixedPoint_t data);
		fixedPoint_t get_data(int k, int v, int width);
		uint8_t* get_bytes(int& length);
		void print(std::ofstream *od);
	
	
	private:
		fixedPoint_t *** m_Data;
		int m_kernels;
		int m_channels;
		int m_filValues;	
		int m_fxPtLen;
		int m_numFracBits;
};
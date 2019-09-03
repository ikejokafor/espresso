#pragma once


const int PIX_SEQ_CONFIG_SIZE = 2;
const int WINDOW_3x3_NUM_CYCLES	= 5;
const int MAX_NUM_INPUT_COLS = 512;
const int NUM_CE_PER_QUAD = 8;


// System Includes
#include <stdint.h>


class pixel_seq_config
{
	public:
		pixel_seq_config(int stride);
		uint8_t* get_bytes(int& length);
		uint8_t* stride1_config(int& length);
		uint8_t* stride2_config(int& length);
	
		int m_stride;
	
};

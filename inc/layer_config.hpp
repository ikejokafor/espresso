#pragma once


// Definitions
#define CONFIG_SIZE 16


// System Includes
#include <stdint.h>


typedef struct
{
	uint64_t pfb_full_count;
	uint64_t stride;
	uint64_t conv_out_fmt;
	uint64_t padding;
	uint64_t num_output_cols;
	uint64_t num_output_rows;
	uint64_t pix_seq_data_full_count;
	uint64_t upsample;
	uint64_t num_expd_input_cols;
	uint64_t num_expd_input_rows;
	uint64_t crpd_input_col_start;
	uint64_t crpd_input_row_start;
	uint64_t crpd_input_col_end;
	uint64_t crpd_input_row_end;
	uint64_t num_kernels;
	uint64_t master_quad;
	uint64_t cascade;
	uint64_t actv;
} layer_config_t;
	

class layer_config
{
	public:
		layer_config(layer_config_t params);
		uint8_t* get_bytes(int& length);
		uint64_t m_pfb_full_count;
		uint64_t m_stride;
		uint64_t m_conv_out_fmt;
		uint64_t m_padding;
		uint64_t m_num_output_cols;
		uint64_t m_num_output_rows;
		uint64_t m_pix_seq_data_full_count;
		uint64_t m_upsample;
		uint64_t m_num_expd_input_cols;
		uint64_t m_num_expd_input_rows;
		uint64_t m_crpd_input_col_start;
		uint64_t m_crpd_input_row_start;
		uint64_t m_crpd_input_col_end;
		uint64_t m_crpd_input_row_end;
		uint64_t m_num_kernels;
		uint64_t m_master_quad;
		uint64_t m_cascade;
		uint64_t m_actv;
};
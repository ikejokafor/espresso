#include "layer_config.hpp"


layer_config::layer_config(layer_config_t params)
{
	m_pfb_full_count = params.pfb_full_count;
	m_stride = params.stride;
	m_conv_out_fmt = params.conv_out_fmt;
	m_padding = params.padding;
	m_num_output_cols = params.num_output_cols;
	m_num_output_rows = params.num_output_rows;
	m_pix_seq_data_full_count = params.pix_seq_data_full_count;
	m_upsample = params.upsample;
	m_num_expd_input_cols = params.num_expd_input_cols;
	m_num_expd_input_rows = params.num_expd_input_rows;
	m_crpd_input_col_start = params.crpd_input_col_start;
	m_crpd_input_row_start = params.crpd_input_row_start;
	m_crpd_input_col_end = params.crpd_input_col_end;
	m_crpd_input_row_end = params.crpd_input_row_end;
	m_num_kernels = params.num_kernels;
	m_master_quad = params.master_quad;
	m_cascade = params.cascade;
	m_actv = params.actv;
}


uint8_t* layer_config::get_bytes(int& length)
{
	length = CONFIG_SIZE;
	uint64_t* bytes = new uint64_t[length];
	bytes[0] = m_pfb_full_count
		| (m_stride << 10)
		| (m_conv_out_fmt << 17)
		| (m_padding << 18)
		| (m_num_output_cols << 19)
		| (m_num_output_rows << 29)
		| (m_pix_seq_data_full_count << 39)
		| (m_upsample << 51)
		| (m_num_expd_input_cols << 52);
	bytes[1] = m_num_expd_input_rows
		| (m_crpd_input_row_end << 10)
		| (m_crpd_input_col_start << 20)
		| (m_crpd_input_row_start << 30)
		| (m_crpd_input_col_end << 40)
		| (m_num_kernels << 50)
		| (m_master_quad << 60)
		| (m_cascade << 61)
		| (m_actv << 62);
	return (uint8_t*)bytes;
}
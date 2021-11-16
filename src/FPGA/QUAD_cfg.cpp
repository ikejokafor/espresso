#include "QUAD_cfg.hpp"
using namespace espresso;


QUAD_cfg::QUAD_cfg() { }


QUAD_cfg::QUAD_cfg(
    int FAS_id,
    int AWP_id,
    int QUAD_id,
    bool QUAD_en,
    int numInputMapRows,
    int numInputMapCols,
    int numKernels,
    int kernel3x3Depth,
    int numKernelRows,
    int numKernelCols,
    int stride,
    bool upsample,
    int padding,
    activation_t act3x3,
    bool it_act3x3,
    bool it_bias3x3,
    bool master_QUAD,
    bool cascade,
    int inMapDepth,
    int res_high_watermark,
    int group
) {
	m_FAS_id = FAS_id;
	m_AWP_id = AWP_id;
	m_QUAD_id = QUAD_id;
    m_inMapDepth = inMapDepth;
    m_it_act3x3 = it_act3x3;
    m_it_bias3x3 = it_bias3x3;
	if(!QUAD_en)
	{
		return;
	}
    m_master_QUAD = master_QUAD;
    m_cascade = cascade;
    m_stride = stride;
    m_act3x3 = act3x3;
    m_padding = (padding > 0) ? true : false;
    m_upsample = upsample;
    int kernel_size = 3;
	m_num_kernels = numKernels;
    
    m_num_input_rows        = numInputMapRows;
    m_num_input_cols        = numInputMapCols;
    m_kernel3x3Depth        = kernel3x3Depth;
    
    m_num_output_rows = ((numInputMapRows - kernel_size + (2 * padding)) / m_stride) + 1;
    m_num_output_cols = ((numInputMapCols - kernel_size + (2 * padding)) / m_stride) + 1;
    if(m_padding && !m_upsample)
    {
        m_num_expd_input_rows = numInputMapRows + 2;
        m_num_expd_input_cols = numInputMapCols + 2;
    }
    else if(!m_padding && m_upsample)
    {
        m_num_expd_input_rows = numInputMapRows * 2;
        m_num_expd_input_cols = numInputMapCols * 2;
        m_num_output_rows = ((m_num_expd_input_rows - kernel_size + (2 * m_padding)) / m_stride) + 1;
        m_num_output_cols = ((m_num_expd_input_cols - kernel_size + (2 * m_padding)) / m_stride) + 1;
    }
    else if(m_padding && m_upsample)
    {
        m_num_expd_input_rows = (numInputMapRows * 2) + 2;
        m_num_expd_input_cols = (numInputMapCols * 2) + 2;
        m_num_output_rows = (((m_num_expd_input_rows - 2) - kernel_size + (2 * m_padding)) / m_stride) + 1;
        m_num_output_cols = (((m_num_expd_input_cols - 2) - kernel_size + (2 * m_padding)) / m_stride) + 1;
    }
    else // !m_padding && !m_upsample
    {
        m_num_expd_input_rows = numInputMapRows;
        m_num_expd_input_cols = numInputMapCols;
    }
	m_crpd_input_col_start = 1;
	m_crpd_input_row_start = 1;
	m_crpd_input_row_end = m_num_expd_input_rows - 2;
	m_crpd_input_col_end = m_num_expd_input_cols - 2;
    m_res_high_watermark = res_high_watermark;
    m_group = group;
}


QUAD_cfg::~QUAD_cfg()
{

}

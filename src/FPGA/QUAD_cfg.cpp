#include "QUAD_cfg.hpp"


QUAD_cfg::QUAD_cfg(
    int FAS_id,	
    int AWP_id,
    int QUAD_id, 		
    int numInputMapRows, 
    int numInputMapCols, 
    int numKernels, 
    int kernelDepth, 
    int numKernelRows, 
    int numKernelCols,
    int stride,
    bool upsample,
    int padding,
    bool conv_out_fmt0,
    bool activation,
    bool master_quad,
    bool cascade
) {
    m_master_quad = master_quad;
    m_cascade = cascade;
    m_FAS_id = FAS_id;
    m_AWP_id = AWP_id;
    m_QUAD_id = QUAD_id; 
    m_stride = stride;
    m_conv_out_fmt0 = conv_out_fmt0;
    m_activation = activation;
    m_padding = padding;
    m_upsample = upsample;
    int kernel_size = 3;
	m_num_kernels = numKernels;
    m_num_output_rows = ((numInputMapRows - kernel_size + (2 * m_padding)) / m_stride) + 1;
    m_num_output_col = ((numInputMapCols - kernel_size + (2 * m_padding)) / m_stride) + 1;     
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
        m_num_output_col = ((m_num_expd_input_cols - kernel_size + (2 * m_padding)) / m_stride) + 1;
    }
    else if(m_padding && m_upsample)
    {
        m_num_expd_input_rows = (numInputMapRows * 2) + 2;
        m_num_expd_input_cols = (numInputMapCols * 2) + 2;
        m_num_output_rows = (((m_num_expd_input_rows - 2) - kernel_size + (2 * m_padding)) / m_stride) + 1;
        m_num_output_col = (((m_num_expd_input_cols - 2) - kernel_size + (2 * m_padding)) / m_stride) + 1;        
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
}


QUAD_cfg::~QUAD_cfg()
{
    
}

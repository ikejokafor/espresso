#pragma once


#include <vector>
#include "common.hpp"


class QUAD_cfg
{
    public:
		QUAD_cfg();
	    QUAD_cfg(
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
		);
        ~QUAD_cfg();

		int 	m_FAS_id;
        int 	m_AWP_id;
		int 	m_QUAD_id;
		int 	m_stride			    ;
		int		m_num_output_rows		;
		int		m_num_output_col	    ;
		int		m_num_kernels			;
		bool    m_master_quad			;
		bool	m_cascade				;
		int     m_num_expd_input_rows	;
		int		m_num_expd_input_cols	;
		bool	m_conv_out_fmt0			;
		bool	m_activation			;
		bool	m_padding				;
		bool	m_upsample				;
		int 	m_crpd_input_row_start	;
		int 	m_crpd_input_col_start	;
		int 	m_crpd_input_row_end	;
		int 	m_crpd_input_col_end	;
};
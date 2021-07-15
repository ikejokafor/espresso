#pragma once


#include <vector>
#include "espresso_FPGA_common.hpp"
#include "espresso_common.hpp"


class QUAD_cfg
{
    public:
        QUAD_cfg();
        QUAD_cfg(
            int FAS_id,
            int AWP_id,
            int QUAD_id,
            bool QUAD_en,
            int numInputMapRows = 0,
            int numInputMapCols = 0,
            int numKernels = 0,
            int kernelDepth = 0,
            int numKernelRows = 0,
            int numKernelCols = 0,
            int stride = 0,
            bool upsample = false,
            int padding = 0,
            espresso::activation_t act3x3 = espresso::NONE,
            bool it_act3x3 = false,
            bool it_bias3x3 = false,
            bool master_QUAD = false,
            bool cascade = false,
            int inMapDepth = 0
#ifdef SYSTEMC
			,			
            int res_high_watermark = 0
#endif
        );
        ~QUAD_cfg();

        int 	m_FAS_id                ;
        int 	m_AWP_id                ;
        int 	m_QUAD_id               ;
        int		m_inMapDepth			;
        int 	m_stride			    ;
        int		m_num_output_rows		;
        int		m_num_output_cols	    ;
        int		m_num_kernels			;
        int     m_kernel3x3Depth        ;
        bool    m_master_QUAD			;
        bool	m_cascade				;
        int     m_num_expd_input_rows	;
        int		m_num_expd_input_cols	;
        int     m_num_input_rows        ;
        int     m_num_input_cols        ;
        espresso::activation_t	m_act3x3    	;
        bool    m_it_act3x3             ;
        bool    m_it_bias3x3            ;
        bool	m_padding				;
        bool	m_upsample				;
        int 	m_crpd_input_row_start	;
        int 	m_crpd_input_col_start	;
        int 	m_crpd_input_row_end	;
        int 	m_crpd_input_col_end	;
#ifdef SYSTEMC
        int     m_res_high_watermark    ;
#endif
};
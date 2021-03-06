#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fixedPoint.hpp"
#include "FPGA_shim.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"
#include "util.hpp"
#include "espresso_FPGA_common.hpp"
#include "espresso_common.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif


class AccelConfig : public Accel_Payload
{
	public:
		AccelConfig(FPGA_hndl* fpga_hndl);
		~AccelConfig();
		void serialize();
        void deserialize();
		void createCfg();

		typedef struct
		{
			uint64_t FAS_id                     ;
			uint64_t AWP_id                     ;
			uint64_t AWP_en                     ;
			uint64_t opcode                  	;
			uint64_t pixSeqCfgFetchTotal		;
			uint64_t inMapFetchTotal          	;
			uint64_t krnl3x3FetchTotal          ;
			uint64_t krnl1x1FetchTotal          ;
			uint64_t krnl3x3BiasFetchTotal      ;
			uint64_t krnl1x1BiasFetchTotal      ;
			uint64_t resMapFetchTotal         	;
			uint64_t partMapFetchTotal        	;
			uint64_t outMapStoreTotal			;
            uint64_t prevMapFetchTotal          ;
			uint64_t inMapFetchFactor			;
			uint64_t outMapStoreFactor			;
			uint64_t co_high_watermark			;
			uint64_t rm_low_watermark			;
			uint64_t pm_low_watermark			;
            uint64_t pv_low_watermark           ;
			uint64_t rm_ftch_vld_total			;
			uint64_t pv_ftch_vld_total			;
			uint64_t pm_ftch_vld_total			;
            uint64_t res_high_watermark         ;
			uint64_t krnl1x1_pding				;
			uint64_t krnl1x1_pad_bgn			;
			uint64_t krnl1x1_pad_end			;
			uint64_t inMapAddr	                ;
            uint64_t inMapFetchAmt              ;
			uint64_t krnl3x3Addr	            ;
			uint64_t krnl3x3BiasAddr	        ;
			uint64_t num_1x1_kernels			;
			uint64_t krnl1x1Depth				;
			uint64_t partMapAddr				;
			uint64_t resMapAddr					;
			uint64_t outMapAddr					;
			uint64_t pixelSeqAddr               ;
			uint64_t prevMapAddr				;
			uint64_t inMapDepth					;
			uint64_t QUAD_id                    ;
			uint64_t QUAD_en                    ;
			uint64_t stride                     ;
			uint64_t num_output_rows            ;
			uint64_t num_output_cols            ;
			uint64_t num_kernels                ;
			uint64_t master_QUAD                ;
			uint64_t cascade                    ;
			uint64_t num_expd_input_rows        ;
			uint64_t num_expd_input_cols        ;
            uint64_t num_input_rows             ;
            uint64_t num_input_cols             ;
            uint64_t kernel3x3Depth             ;
			uint64_t activation                 ;
            uint64_t act3x3                     ;
            uint64_t act1x1                     ;
            uint64_t it_act1x1                  ;
            uint64_t it_act3x3                  ;
            uint64_t it_bias1x1                 ;
            uint64_t it_bias3x3                 ;
			uint64_t padding                    ;
			uint64_t upsample                   ;
			uint64_t crpd_input_row_start       ;
			uint64_t crpd_input_col_start       ;
			uint64_t crpd_input_row_end         ;
			uint64_t crpd_input_col_end         ;
			uint64_t fas_num_output_rows    	;
			uint64_t fas_num_output_cols	    ;
            uint64_t fas_outputh_depth          ;
            uint64_t om_store_vld_total         ;
			uint64_t quad_num_output_rows	    ;
			uint64_t quad_num_output_cols	    ;
#ifdef SYSTEMC
            uint64_t kernel_i                   ;
            uint64_t depth_i                    ;
            char layerName[32]                  ;
            bool last                           ;
            bool first                          ;
#endif
			
		} cfg_t;

        
		std::vector<FAS_cfg*> m_FAS_cfg_arr;
        FPGA_hndl* m_fpga_hndl;
};

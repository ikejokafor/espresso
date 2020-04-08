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


class AccelConfig : public Accel_Payload
{
	public:
		AccelConfig();
		~AccelConfig();
		uint64_t allocate(int size);
		void deallocate();
		void serialize();
        void deserialize();
		void createCfg();

		typedef struct
		{
			uint64_t FAS_id                     ;
			uint64_t AWP_id                     ;
			uint64_t AWP_en                     ;
			uint64_t do_res_layer               ;
			uint64_t first_depth_iter           ;
			uint64_t last_depth_iter            ;
			uint64_t do_kernels1x1              ;
			uint64_t pixSeqCfgFetchTotal		;
			uint64_t inMapFetchTotal          	;
			uint64_t krnl3x3FetchTotal          ;
			uint64_t krnl1x1FetchTotal          ;
			uint64_t krnl3x3BiasFetchTotal      ;
			uint64_t krnl1x1BiasFetchTotal      ;
			uint64_t resMapFetchTotal         	;
			uint64_t partMapFetchTotal        	;
			uint64_t outMapStoreTotal			;
			uint64_t inMapFetchFactor			;
			uint64_t imAddrArr	                ;
			uint64_t krnl3x3Addr	            ;
			uint64_t krnl3x3BiasAddr	        ;
			uint64_t krnl1x1BiasAddr            ;
			uint64_t partMapAddr				;
			uint64_t resMapAddr					;
			uint64_t outMapAddr					;
			uint64_t pixelSeqAddr               ;
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
			uint64_t activation                 ;
			uint64_t padding                    ;
			uint64_t upsample                   ;
			uint64_t crpd_input_row_start       ;
			uint64_t crpd_input_col_start       ;
			uint64_t crpd_input_row_end         ;
			uint64_t crpd_input_col_end         ;
		} cfg_t;

		std::vector<FAS_cfg*> m_FAS_cfg_arr;
};

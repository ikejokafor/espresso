#pragma once


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
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

		typedef struct
		{
			uint64_t do_res_layer               ;
			uint64_t first_depth_iter           ;
			uint64_t last_depth_iter            ;
			uint64_t do_kernel1x1               ;
			uint64_t inMapFetchTotal          	;
			uint64_t partMapFetchTotal        	;
			uint64_t krnl1x1FetchTotal          ;
			uint64_t resMapFetchTotal         	;
			uint64_t outMapFetchTotal			;
			uint64_t imAddrArr_0                ;
			uint64_t imAddrArr_1                ;
			uint64_t imAddrArr_2                ;
			uint64_t imAddrArr_3                ;
			uint64_t krnl3x3Addr_0              ;
			uint64_t krnl3x3Addr_1              ;
			uint64_t krnl3x3Addr_2              ;
			uint64_t krnl3x3Addr_3              ;
			uint64_t krnl1x1AddrArr             ;
			uint64_t partMapAddr				;
			uint64_t resMapAddr					;
			uint64_t biasAddr                   ;
			uint64_t pixelSeqAddr               ;
			uint64_t FAS_id                     ;
			uint64_t AWP_id                     ;
			uint64_t QUAD_id                    ;
			uint64_t AWP_en                     ;
			uint64_t QUAD_en                    ;
			uint64_t stride                     ;
			uint64_t num_output_rows            ;
			uint64_t num_output_col             ;
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

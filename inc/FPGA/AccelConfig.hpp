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
			uint32_t res_layer                  ;
			uint32_t first_iter                 ;
			uint32_t last_iter                  ;
			uint32_t kernel_1x1                 ;
			uint32_t partMapFetchTotal        	;
			uint32_t inMapFetchTotal          	;
			uint32_t krnlFetchTotal           	;
			uint32_t resMapFetchTotal         	;
			uint32_t imAddrArr_0                ;
			uint32_t imAddrArr_1                ;
			uint32_t imAddrArr_2                ;
			uint32_t imAddrArr_3                ;
			uint32_t krnl3x3AddrArr             ;
			uint32_t krnl1x1AddrArr             ;
			uint32_t biasAddr                   ;
			uint32_t pixelSeqAddr               ;
			uint32_t FAS_id                     ;
			uint32_t AWP_id                     ;
			uint32_t QUAD_id                    ;
			uint32_t AWP_en                     ;
			uint32_t QUAD_en                    ;
			uint32_t stride                     ;
			uint32_t num_output_rows            ;
			uint32_t num_output_col             ;
			uint32_t num_kernels                ;
			uint32_t master_QUAD                ;
			uint32_t cascade                    ;
			uint32_t num_expd_input_rows        ;
			uint32_t num_expd_input_cols        ;
			uint32_t activation                 ;
			uint32_t padding                    ;
			uint32_t upsample                   ;
			uint32_t crpd_input_row_start       ;
			uint32_t crpd_input_col_start       ;
			uint32_t crpd_input_row_end         ;
			uint32_t crpd_input_col_end         ;
		} cfg_t;

		std::vector<FAS_cfg*> m_FAS_cfg_arr;
};

#include "AccelConfig.hpp"
using namespace std;


AccelConfig::AccelConfig() : Accel_Payload()
{
	m_address = allocate(MAX_AWP_PER_FAS * NUM_QUAD_PER_AWP * sizeof(cfg_t));
}


AccelConfig::~AccelConfig()
{
	deallocate();
}


uint64_t AccelConfig::allocate(int size)
{
#ifdef SYSTEMC
	m_size = size;
	return (uint64_t)malloc(size);
#else

#endif
}


void AccelConfig::deallocate()
{
#ifdef SYSTEMC
	free((uint64_t*)m_address);
#else

#endif
}


void AccelConfig::serialize()
{
#ifdef SYSTEMC
	cfg_t* cfg = (cfg_t*)m_address;
	// TODO: add support for multiple FAS's
	for (int a = 0; a < MAX_AWP_PER_FAS; a++)
	{
		auto AWP_en_arr = m_FAS_cfg_arr[0]->m_AWP_en_arr;
		auto QUAD_cfg_arr = m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;	
		for (int q = 0; q < NUM_QUAD_PER_AWP; q++)
		{
			int idx 							= index2D(NUM_QUAD_PER_AWP, a, q);
			cfg[idx].do_res_layer 				= m_FAS_cfg_arr[0]->m_do_res_layer;
			cfg[idx].first_depth_iter 			= m_FAS_cfg_arr[0]->m_first_depth_iter;
			cfg[idx].last_depth_iter 			= m_FAS_cfg_arr[0]->m_last_depth_iter;
			cfg[idx].do_kernel1x1 				= m_FAS_cfg_arr[0]->m_do_kernel1x1;
			cfg[idx].inMapFetchTotal        	= m_FAS_cfg_arr[0]->m_inMapFetchTotal;
			cfg[idx].partMapFetchTotal      	= m_FAS_cfg_arr[0]->m_partMapFetchTotal;
			cfg[idx].krnl1x1FetchTotal         	= m_FAS_cfg_arr[0]->m_krnl1x1FetchTotal;
			cfg[idx].resMapFetchTotal       	= m_FAS_cfg_arr[0]->m_resMapFetchTotal;
			cfg[idx].outMapFetchTotal			= m_FAS_cfg_arr[0]->m_outMapStoreTotal;
			cfg[idx].imAddrArr_0 				= m_FAS_cfg_arr[0]->m_imAddrArr[0];
			cfg[idx].imAddrArr_1 				= m_FAS_cfg_arr[0]->m_imAddrArr[1];
			cfg[idx].imAddrArr_2 				= m_FAS_cfg_arr[0]->m_imAddrArr[2];
			cfg[idx].imAddrArr_3 				= m_FAS_cfg_arr[0]->m_imAddrArr[3];
			cfg[idx].krnl3x3Addr_0	 			= m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[0];
			cfg[idx].krnl3x3Addr_1	 			= m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[1];
			cfg[idx].krnl3x3Addr_2	 			= m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[2];
			cfg[idx].krnl3x3Addr_3	 			= m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[3];
			cfg[idx].krnl1x1AddrArr 			= m_FAS_cfg_arr[0]->m_krnl1x1AddrArr;
			cfg[idx].partMapAddr				= m_FAS_cfg_arr[0]->m_partMapAddr;
			cfg[idx].resMapAddr					= m_FAS_cfg_arr[0]->m_resMapAddr;
			cfg[idx].biasAddr 					= m_FAS_cfg_arr[0]->m_biasAddr;
			cfg[idx].pixelSeqAddr 				= m_FAS_cfg_arr[0]->m_pixelSeqAddr;
			cfg[idx].FAS_id 					= QUAD_cfg_arr[q]->m_FAS_id;
			cfg[idx].AWP_id 					= QUAD_cfg_arr[q]->m_AWP_id;
			cfg[idx].QUAD_id 					= QUAD_cfg_arr[q]->m_QUAD_id;
			cfg[idx].AWP_en						= AWP_en_arr[a];
			cfg[idx].QUAD_en					= QUAD_cfg_arr[q]->m_QUAD_en;
			cfg[idx].stride 					= QUAD_cfg_arr[q]->m_stride;
			cfg[idx].num_output_rows 			= QUAD_cfg_arr[q]->m_num_output_rows;
			cfg[idx].num_expd_input_cols 		= QUAD_cfg_arr[q]->m_num_output_col;
			cfg[idx].num_kernels 				= QUAD_cfg_arr[q]->m_num_kernels;
			cfg[idx].master_QUAD 				= QUAD_cfg_arr[q]->m_master_QUAD;
			cfg[idx].cascade 					= QUAD_cfg_arr[q]->m_cascade;
			cfg[idx].num_expd_input_rows 		= QUAD_cfg_arr[q]->m_num_expd_input_rows;
			cfg[idx].num_expd_input_cols 		= QUAD_cfg_arr[q]->m_num_expd_input_cols;
			cfg[idx].activation 				= QUAD_cfg_arr[q]->m_activation;
			cfg[idx].padding 					= QUAD_cfg_arr[q]->m_padding;
			cfg[idx].upsample 					= QUAD_cfg_arr[q]->m_upsample;
			cfg[idx].crpd_input_row_start 		= QUAD_cfg_arr[q]->m_crpd_input_row_start;
			cfg[idx].crpd_input_col_start 		= QUAD_cfg_arr[q]->m_crpd_input_col_start;
			cfg[idx].crpd_input_row_end	    	= QUAD_cfg_arr[q]->m_crpd_input_row_end;
			cfg[idx].crpd_input_col_end			= QUAD_cfg_arr[q]->m_crpd_input_col_end;
		}
	}
#else

#endif
}


void AccelConfig::deserialize()
{
#ifdef SYSTEMC
	cfg_t* cfg = (cfg_t*)m_address;
	// TODO: add support for multiple FAS's
	for (int a = 0; a < MAX_AWP_PER_FAS; a++)
	{
		auto AWP_en_arr = m_FAS_cfg_arr[0]->m_AWP_en_arr;
		auto QUAD_cfg_arr = m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;	
		for (int q = 0; q < NUM_QUAD_PER_AWP; q++)
		{
			int idx 									= index2D(NUM_QUAD_PER_AWP, a, q);
			m_FAS_cfg_arr[0]->m_do_res_layer			= cfg[idx].do_res_layer 			;
			m_FAS_cfg_arr[0]->m_first_depth_iter		= cfg[idx].first_depth_iter 		;
			m_FAS_cfg_arr[0]->m_last_depth_iter			= cfg[idx].last_depth_iter 			;
			m_FAS_cfg_arr[0]->m_do_kernel1x1			= cfg[idx].do_kernel1x1 			;
			m_FAS_cfg_arr[0]->m_imAddrArr[0]			= cfg[idx].imAddrArr_0 				;
			m_FAS_cfg_arr[0]->m_imAddrArr[1]			= cfg[idx].imAddrArr_1 				;
			m_FAS_cfg_arr[0]->m_imAddrArr[2]			= cfg[idx].imAddrArr_2 				;
			m_FAS_cfg_arr[0]->m_imAddrArr[3]			= cfg[idx].imAddrArr_3 				;
			m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[0]		= cfg[idx].krnl3x3Addr_0 			;
			m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[1]		= cfg[idx].krnl3x3Addr_1			;
			m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[2]		= cfg[idx].krnl3x3Addr_2			; 	
			m_FAS_cfg_arr[0]->m_krnl3x3AddrArr[3]		= cfg[idx].krnl3x3Addr_3			; 	
			m_FAS_cfg_arr[0]->m_krnl1x1AddrArr			= cfg[idx].krnl1x1AddrArr 			;
			m_FAS_cfg_arr[0]->m_biasAddr			    = cfg[idx].biasAddr 				;
			m_FAS_cfg_arr[0]->m_pixelSeqAddr			= cfg[idx].pixelSeqAddr 			;
			m_FAS_cfg_arr[0]->m_partMapAddr				= cfg[idx].partMapAddr				;
			m_FAS_cfg_arr[0]->m_resMapAddr				= cfg[idx].resMapAddr				;
			QUAD_cfg_arr[q]->m_FAS_id			        = cfg[idx].FAS_id 					;
			QUAD_cfg_arr[q]->m_AWP_id			        = cfg[idx].AWP_id 					;
			QUAD_cfg_arr[q]->m_QUAD_id			        = cfg[idx].QUAD_id 					;
			AWP_en_arr[a]								= cfg[idx].AWP_en					;
			QUAD_cfg_arr[q]->m_QUAD_en					= cfg[idx].QUAD_en					;
			QUAD_cfg_arr[q]->m_stride			        = cfg[idx].stride 					;
			QUAD_cfg_arr[q]->m_num_output_rows			= cfg[idx].num_output_rows 			;
			QUAD_cfg_arr[q]->m_num_output_col			= cfg[idx].num_expd_input_cols 		;
			QUAD_cfg_arr[q]->m_num_kernels			    = cfg[idx].num_kernels 				;
			QUAD_cfg_arr[q]->m_master_QUAD			    = cfg[idx].master_QUAD				;
			QUAD_cfg_arr[q]->m_cascade			        = cfg[idx].cascade 					;
			QUAD_cfg_arr[q]->m_num_expd_input_rows      = cfg[idx].num_expd_input_rows 		;
			QUAD_cfg_arr[q]->m_num_expd_input_cols	    = cfg[idx].num_expd_input_cols 		;
			QUAD_cfg_arr[q]->m_activation			    = cfg[idx].activation 				;
			QUAD_cfg_arr[q]->m_padding			        = cfg[idx].padding 					;
			QUAD_cfg_arr[q]->m_upsample			        = cfg[idx].upsample 				;
			QUAD_cfg_arr[q]->m_crpd_input_row_start     = cfg[idx].crpd_input_row_start 	;
			QUAD_cfg_arr[q]->m_crpd_input_col_start		= cfg[idx].crpd_input_col_start 	;
			QUAD_cfg_arr[q]->m_crpd_input_row_end		= cfg[idx].crpd_input_row_end	    ;
			QUAD_cfg_arr[q]->m_crpd_input_col_end		= cfg[idx].crpd_input_col_end		;
		}
	}
#else

#endif
}

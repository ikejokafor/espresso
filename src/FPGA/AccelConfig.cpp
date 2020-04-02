#include "AccelConfig.hpp"
using namespace std;


AccelConfig::AccelConfig() : Accel_Payload()
{
	m_address = allocate(MAX_AWP_PER_FAS * NUM_TOTAL_QUADS * sizeof(cfg_t));
}


AccelConfig::~AccelConfig()
{
	deallocate();
}


uint64_t AccelConfig::allocate(int size)
{
	m_size = size;
#ifdef SYSTEMC
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
	for(int f = 0; f < NUM_FAS; f++)
	{
		for (int a = 0; a < MAX_AWP_PER_FAS; a++)
		{
			auto& AWP_en_arr = m_FAS_cfg_arr[f]->m_AWP_en_arr;
			auto& QUAD_cfg_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
			auto& QUAD_en_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;	
			for (int q = 0; q < NUM_TOTAL_QUADS; q++)
			{
				int idx 							= index2D(NUM_TOTAL_QUADS, a, q);
				cfg[idx].FAS_id 					= QUAD_cfg_arr[q]->m_FAS_id;
				cfg[idx].AWP_id 					= QUAD_cfg_arr[q]->m_AWP_id;
				cfg[idx].AWP_en						= AWP_en_arr[a];
				cfg[idx].do_res_layer 				= m_FAS_cfg_arr[f]->m_do_res_layer;
				cfg[idx].first_depth_iter 			= m_FAS_cfg_arr[f]->m_first_depth_iter;
				cfg[idx].last_depth_iter 			= m_FAS_cfg_arr[f]->m_last_depth_iter;
				cfg[idx].do_kernel1x1 				= m_FAS_cfg_arr[f]->m_do_kernel1x1;
				cfg[idx].pixSeqCfgFetchTotal		= m_FAS_cfg_arr[f]->m_pixSeqCfgFetchTotal;
				cfg[idx].inMapFetchTotal        	= m_FAS_cfg_arr[f]->m_inMapFetchTotal;
				cfg[idx].partMapFetchTotal      	= m_FAS_cfg_arr[f]->m_partMapFetchTotal;
				cfg[idx].krnl1x1FetchTotal         	= m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal;
				cfg[idx].resMapFetchTotal       	= m_FAS_cfg_arr[f]->m_resMapFetchTotal;
				cfg[idx].outMapStoreTotal			= m_FAS_cfg_arr[f]->m_outMapStoreTotal;
				cfg[idx].imAddrArr_0 				= m_FAS_cfg_arr[f]->m_imAddrArr[0];
				cfg[idx].imAddrArr_1 				= m_FAS_cfg_arr[f]->m_imAddrArr[1];
				cfg[idx].imAddrArr_2 				= m_FAS_cfg_arr[f]->m_imAddrArr[2];
				cfg[idx].imAddrArr_3 				= m_FAS_cfg_arr[f]->m_imAddrArr[3];
				cfg[idx].krnl3x3Addr_0	 			= m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[0];
				cfg[idx].krnl3x3Addr_1	 			= m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[1];
				cfg[idx].krnl3x3Addr_2	 			= m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[2];
				cfg[idx].krnl3x3Addr_3	 			= m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[3];
				cfg[idx].krnl3x3BiasAddr_0			= m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[0];
				cfg[idx].krnl3x3BiasAddr_1			= m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[1];
				cfg[idx].krnl3x3BiasAddr_2			= m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[2];
				cfg[idx].krnl3x3BiasAddr_3			= m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[3];
				cfg[idx].krnl1x1BiasAddr 			= m_FAS_cfg_arr[f]->m_krnl1x1BiasAddr;
				cfg[idx].partMapAddr				= m_FAS_cfg_arr[f]->m_partMapAddr;
				cfg[idx].resMapAddr					= m_FAS_cfg_arr[f]->m_resMapAddr;
				cfg[idx].outMapAddr					= m_FAS_cfg_arr[f]->m_outMapAddr;
				cfg[idx].pixelSeqAddr 				= m_FAS_cfg_arr[f]->m_pixelSeqAddr;
				cfg[idx].QUAD_id 					= QUAD_cfg_arr[q]->m_QUAD_id;
				cfg[idx].QUAD_en					= QUAD_en_arr[q];
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
	}
#else

#endif
}


void AccelConfig::deserialize()
{
	createCfg();
#ifdef SYSTEMC
	cfg_t* cfg = (cfg_t*)m_address;
	for(int f = 0; f < NUM_FAS; f++)
	{
		for (int a = 0; a < MAX_AWP_PER_FAS; a++)
		{
			auto& AWP_en_arr = m_FAS_cfg_arr[f]->m_AWP_en_arr;
			auto& QUAD_cfg_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
			auto& QUAD_en_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;	
			for (int q = 0; q < NUM_TOTAL_QUADS; q++)
			{
				int idx 										= index2D(NUM_TOTAL_QUADS, a, q);
				QUAD_cfg_arr[q]->m_FAS_id				    = cfg[idx].FAS_id;
				QUAD_cfg_arr[q]->m_AWP_id				    = cfg[idx].AWP_id;
				AWP_en_arr[a]				                = cfg[idx].AWP_en;
				m_FAS_cfg_arr[f]->m_do_res_layer			= cfg[idx].do_res_layer;
				m_FAS_cfg_arr[f]->m_first_depth_iter		= cfg[idx].first_depth_iter;
				m_FAS_cfg_arr[f]->m_last_depth_iter			= cfg[idx].last_depth_iter;
				m_FAS_cfg_arr[f]->m_do_kernel1x1			= cfg[idx].do_kernel1x1;
				m_FAS_cfg_arr[f]->m_inMapFetchTotal			= cfg[idx].inMapFetchTotal;
				m_FAS_cfg_arr[f]->m_partMapFetchTotal		= cfg[idx].partMapFetchTotal;
				m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal		= cfg[idx].krnl1x1FetchTotal;
				m_FAS_cfg_arr[f]->m_resMapFetchTotal		= cfg[idx].resMapFetchTotal;
				m_FAS_cfg_arr[f]->m_outMapStoreTotal		= cfg[idx].outMapStoreTotal;
				m_FAS_cfg_arr[f]->m_imAddrArr[0]			= cfg[idx].imAddrArr_0;
				m_FAS_cfg_arr[f]->m_imAddrArr[1]			= cfg[idx].imAddrArr_1;
				m_FAS_cfg_arr[f]->m_imAddrArr[2]			= cfg[idx].imAddrArr_2;
				m_FAS_cfg_arr[f]->m_imAddrArr[3]			= cfg[idx].imAddrArr_3;
				m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[0]		= cfg[idx].krnl3x3Addr_0;
				m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[1]		= cfg[idx].krnl3x3Addr_1;
				m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[2]		= cfg[idx].krnl3x3Addr_2;
				m_FAS_cfg_arr[f]->m_krnl3x3AddrArr[3]		= cfg[idx].krnl3x3Addr_3;
				m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[0]   = cfg[idx].krnl3x3BiasAddr_0;
				m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[1]	= cfg[idx].krnl3x3BiasAddr_1;
				m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[2]	= cfg[idx].krnl3x3BiasAddr_2;
				m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr[3]	= cfg[idx].krnl3x3BiasAddr_3;
				m_FAS_cfg_arr[f]->m_krnl1x1BiasAddr			= cfg[idx].krnl1x1BiasAddr;
				m_FAS_cfg_arr[f]->m_partMapAddr				= cfg[idx].partMapAddr;
				m_FAS_cfg_arr[f]->m_resMapAddr				= cfg[idx].resMapAddr;
				m_FAS_cfg_arr[f]->m_outMapAddr				= cfg[idx].outMapAddr;
				m_FAS_cfg_arr[f]->m_pixelSeqAddr			= cfg[idx].pixelSeqAddr;
				QUAD_cfg_arr[q]->m_QUAD_id				    = cfg[idx].QUAD_id;
				QUAD_en_arr[q]				                = cfg[idx].QUAD_en;
				QUAD_cfg_arr[q]->m_stride				    = cfg[idx].stride;
				QUAD_cfg_arr[q]->m_num_output_rows			= cfg[idx].num_output_rows;
				QUAD_cfg_arr[q]->m_num_output_col			= cfg[idx].num_expd_input_cols;
				QUAD_cfg_arr[q]->m_num_kernels				= cfg[idx].num_kernels;
				QUAD_cfg_arr[q]->m_master_QUAD				= cfg[idx].master_QUAD;
				QUAD_cfg_arr[q]->m_cascade				    = cfg[idx].cascade;
				QUAD_cfg_arr[q]->m_num_expd_input_rows		= cfg[idx].num_expd_input_rows;
				QUAD_cfg_arr[q]->m_num_expd_input_cols		= cfg[idx].num_expd_input_cols;
				QUAD_cfg_arr[q]->m_activation				= cfg[idx].activation;
				QUAD_cfg_arr[q]->m_padding				    = cfg[idx].padding;
				QUAD_cfg_arr[q]->m_upsample				    = cfg[idx].upsample;
				QUAD_cfg_arr[q]->m_crpd_input_row_start		= cfg[idx].crpd_input_row_start;
				QUAD_cfg_arr[q]->m_crpd_input_col_start		= cfg[idx].crpd_input_col_start;
				QUAD_cfg_arr[q]->m_crpd_input_row_end		= cfg[idx].crpd_input_row_end;
				QUAD_cfg_arr[q]->m_crpd_input_col_end		= cfg[idx].crpd_input_col_end;
			}
		}
	}
#else

#endif
}


void AccelConfig::createCfg()
{
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_FAS_cfg_arr.push_back(new FAS_cfg());
		for(int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			m_FAS_cfg_arr[i]->m_AWP_cfg_arr.push_back(new AWP_cfg());
			m_FAS_cfg_arr[i]->m_AWP_en_arr.push_back(false);
			for(int k = 0; k < NUM_TOTAL_QUADS; k++)
			{
				m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr.push_back(new QUAD_cfg());
				m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_en_arr.push_back(false);
			}
		}
	}
}

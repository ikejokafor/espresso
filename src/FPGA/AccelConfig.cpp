#include "AccelConfig.hpp"
using namespace std;


AccelConfig::AccelConfig() : Accel_Payload()
{
    m_address = allocate(MAX_AWP_PER_FAS * NUM_TOTAL_QUADS * sizeof(cfg_t));
}


AccelConfig::~AccelConfig()
{
    deallocate();
    int i_end = m_FAS_cfg_arr.size();
    for(int i = 0; i < i_end; i++)
    {
        int j_end = m_FAS_cfg_arr[i]->m_AWP_cfg_arr.size();
        for(int j = 0; j < j_end; j++)
        {
            int k_end = m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr.size();
            for(int k = 0; k < k_end; k++)
            {
                delete m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr[k];
            }
            delete m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j];
        }
        delete m_FAS_cfg_arr[i];
    }
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
            auto& inMapAddrArr = m_FAS_cfg_arr[f]->m_inMapAddrArr;
            auto& krnl3x3AddrArr = m_FAS_cfg_arr[f]->m_krnl3x3AddrArr;
            auto& krnl3x3BiasAddrArr = m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr;
            auto& QUAD_cfg_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
            auto& QUAD_en_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
            for (int q = 0; q < NUM_TOTAL_QUADS; q++)
            {
                int idx 							= index2D(NUM_TOTAL_QUADS, a, q);
                cfg[idx].FAS_id 					= QUAD_cfg_arr[q]->m_FAS_id;
                cfg[idx].AWP_id 					= QUAD_cfg_arr[q]->m_AWP_id;
                cfg[idx].AWP_en						= AWP_en_arr[a];
                cfg[idx].opcode                     = m_FAS_cfg_arr[f]->m_opcode;
                cfg[idx].pixSeqCfgFetchTotal		= m_FAS_cfg_arr[f]->m_pixSeqCfgFetchTotal;
                cfg[idx].inMapFetchTotal        	= m_FAS_cfg_arr[f]->m_inMapFetchTotal;
                cfg[idx].krnl3x3FetchTotal			= m_FAS_cfg_arr[f]->m_krnl3x3FetchTotal;
                cfg[idx].krnl1x1FetchTotal          = m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal;
                cfg[idx].krnl3x3BiasFetchTotal		= m_FAS_cfg_arr[f]->m_krnl3x3BiasFetchTotal;
                cfg[idx].krnl1x1BiasFetchTotal      = m_FAS_cfg_arr[f]->m_krnl1x1BiasFetchTotal;
                cfg[idx].krnl1x1FetchTotal         	= m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal;
                cfg[idx].num_1x1_kernels            = m_FAS_cfg_arr[f]->m_num_1x1_kernels;
                cfg[idx].krnl1x1Depth               = m_FAS_cfg_arr[f]->m_krnl1x1Depth;
                cfg[idx].resMapFetchTotal       	= m_FAS_cfg_arr[f]->m_resMapFetchTotal;
                cfg[idx].partMapFetchTotal      	= m_FAS_cfg_arr[f]->m_partMapFetchTotal;
                cfg[idx].outMapStoreTotal			= m_FAS_cfg_arr[f]->m_outMapStoreTotal;
                cfg[idx].prevMapFetchTotal          = m_FAS_cfg_arr[f]->m_prevMapFetchTotal;
                cfg[idx].co_high_watermark			= m_FAS_cfg_arr[f]->m_co_high_watermark;
                cfg[idx].rm_low_watermark           = m_FAS_cfg_arr[f]->m_rm_low_watermark;
                cfg[idx].pm_low_watermark           = m_FAS_cfg_arr[f]->m_pm_low_watermark;
                cfg[idx].pv_low_watermark           = m_FAS_cfg_arr[f]->m_pv_low_watermark;
	            cfg[idx].rm_fetch_amount            = m_FAS_cfg_arr[f]->m_rm_fetch_amount;
	            cfg[idx].pm_fetch_amount            = m_FAS_cfg_arr[f]->m_pm_fetch_amount;
	            cfg[idx].pv_fetch_amount            = m_FAS_cfg_arr[f]->m_pv_fetch_amount;
                cfg[idx].krnl1x1_pding				= m_FAS_cfg_arr[f]->m_krnl1x1_pding;
                cfg[idx].krnl1x1_pad_bgn			= m_FAS_cfg_arr[f]->m_krnl1x1_pad_bgn;
                cfg[idx].krnl1x1_pad_end			= m_FAS_cfg_arr[f]->m_krnl1x1_pad_end;
                cfg[idx].inMapFetchFactor           = m_FAS_cfg_arr[f]->m_inMapFetchFactor;
                cfg[idx].outMapStoreFactor          = m_FAS_cfg_arr[f]->m_outMapStoreFactor;
                cfg[idx].imAddrArr 					= inMapAddrArr[a][q];
                cfg[idx].krnl3x3Addr	 			= krnl3x3AddrArr[a][q];
                cfg[idx].krnl3x3BiasAddr			= krnl3x3BiasAddrArr[a][q];
                cfg[idx].krnl1x1Addr                = m_FAS_cfg_arr[f]->m_krnl1x1Addr;
                cfg[idx].krnl1x1BiasAddr 			= m_FAS_cfg_arr[f]->m_krnl1x1BiasAddr;
                cfg[idx].partMapAddr				= m_FAS_cfg_arr[f]->m_partMapAddr;
                cfg[idx].resMapAddr					= m_FAS_cfg_arr[f]->m_resMapAddr;
                cfg[idx].outMapAddr					= m_FAS_cfg_arr[f]->m_outMapAddr;
                cfg[idx].pixelSeqAddr 				= m_FAS_cfg_arr[f]->m_pixelSeqAddr;
                cfg[idx].QUAD_id 					= QUAD_cfg_arr[q]->m_QUAD_id;
                cfg[idx].res_high_watermark         = QUAD_cfg_arr[q]->m_res_high_watermark;
                cfg[idx].QUAD_en					= QUAD_en_arr[q];
                cfg[idx].inMapDepth                 = QUAD_cfg_arr[q]->m_inMapDepth;
                cfg[idx].stride 					= QUAD_cfg_arr[q]->m_stride;
                cfg[idx].num_output_rows 			= QUAD_cfg_arr[q]->m_num_output_rows;
                cfg[idx].num_output_cols	        = QUAD_cfg_arr[q]->m_num_output_cols;
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
            auto& inMapAddrArr = m_FAS_cfg_arr[f]->m_inMapAddrArr;
            auto& krnl3x3AddrArr = m_FAS_cfg_arr[f]->m_krnl3x3AddrArr;
            auto& krnl3x3BiasAddrArr = m_FAS_cfg_arr[f]->m_krnl3x3BiasAddrArr;
            auto& QUAD_cfg_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
            auto& QUAD_en_arr = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
            for (int q = 0; q < NUM_TOTAL_QUADS; q++)
            {
                int idx 								    = index2D(NUM_TOTAL_QUADS, a, q);
                QUAD_cfg_arr[q]->m_FAS_id				    = cfg[idx].FAS_id;
                QUAD_cfg_arr[q]->m_AWP_id				    = cfg[idx].AWP_id;
                AWP_en_arr[a]				                = cfg[idx].AWP_en;
                m_FAS_cfg_arr[f]->m_opcode                  = (opcode_t)cfg[idx].opcode;
                m_FAS_cfg_arr[f]->m_pixSeqCfgFetchTotal		= cfg[idx].pixSeqCfgFetchTotal;
                m_FAS_cfg_arr[f]->m_inMapFetchTotal			= cfg[idx].inMapFetchTotal;
                m_FAS_cfg_arr[f]->m_krnl3x3FetchTotal		= cfg[idx].krnl3x3FetchTotal;
                m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal       = cfg[idx].krnl1x1FetchTotal;
                m_FAS_cfg_arr[f]->m_krnl3x3BiasFetchTotal   = cfg[idx].krnl3x3BiasFetchTotal;
                m_FAS_cfg_arr[f]->m_krnl1x1BiasFetchTotal   = cfg[idx].krnl1x1BiasFetchTotal;
                m_FAS_cfg_arr[f]->m_krnl1x1FetchTotal		= cfg[idx].krnl1x1FetchTotal;
                m_FAS_cfg_arr[f]->m_num_1x1_kernels         = cfg[idx].num_1x1_kernels;
                m_FAS_cfg_arr[f]->m_krnl1x1Depth            = cfg[idx].krnl1x1Depth;
                m_FAS_cfg_arr[f]->m_resMapFetchTotal		= cfg[idx].resMapFetchTotal;
                m_FAS_cfg_arr[f]->m_partMapFetchTotal		= cfg[idx].partMapFetchTotal;
                m_FAS_cfg_arr[f]->m_outMapStoreTotal		= cfg[idx].outMapStoreTotal;
                m_FAS_cfg_arr[f]->m_prevMapFetchTotal       = cfg[idx].prevMapFetchTotal;
                m_FAS_cfg_arr[f]->m_inMapFetchFactor        = cfg[idx].inMapFetchFactor;
                m_FAS_cfg_arr[f]->m_outMapStoreFactor       = cfg[idx].outMapStoreFactor;
                m_FAS_cfg_arr[f]->m_co_high_watermark       = cfg[idx].co_high_watermark;
                m_FAS_cfg_arr[f]->m_rm_low_watermark        = cfg[idx].rm_low_watermark;
                m_FAS_cfg_arr[f]->m_pm_low_watermark        = cfg[idx].pm_low_watermark;
                m_FAS_cfg_arr[f]->m_pv_low_watermark        = cfg[idx].pv_low_watermark;
	            m_FAS_cfg_arr[f]->m_rm_fetch_amount         = cfg[idx].rm_fetch_amount; 
	            m_FAS_cfg_arr[f]->m_pm_fetch_amount         = cfg[idx].pm_fetch_amount; 
	            m_FAS_cfg_arr[f]->m_pv_fetch_amount         = cfg[idx].pv_fetch_amount;
                m_FAS_cfg_arr[f]->m_krnl1x1_pding           = cfg[idx].krnl1x1_pding;
                m_FAS_cfg_arr[f]->m_krnl1x1_pad_bgn         = cfg[idx].krnl1x1_pad_bgn;
                m_FAS_cfg_arr[f]->m_krnl1x1_pad_end         = cfg[idx].krnl1x1_pad_end;
                inMapAddrArr[a][q]				            = cfg[idx].imAddrArr;
                krnl3x3AddrArr[a][q]				        = cfg[idx].krnl3x3Addr;
                krnl3x3BiasAddrArr[a][q]				    = cfg[idx].krnl3x3BiasAddr;
                m_FAS_cfg_arr[f]->m_krnl1x1Addr             = cfg[idx].krnl1x1Addr;
                m_FAS_cfg_arr[f]->m_krnl1x1BiasAddr			= cfg[idx].krnl1x1BiasAddr;
                m_FAS_cfg_arr[f]->m_partMapAddr				= cfg[idx].partMapAddr;
                m_FAS_cfg_arr[f]->m_resMapAddr				= cfg[idx].resMapAddr;
                m_FAS_cfg_arr[f]->m_outMapAddr				= cfg[idx].outMapAddr;
                m_FAS_cfg_arr[f]->m_pixelSeqAddr			= cfg[idx].pixelSeqAddr;
                QUAD_cfg_arr[q]->m_QUAD_id				    = cfg[idx].QUAD_id;
                QUAD_cfg_arr[q]->m_res_high_watermark       = cfg[idx].res_high_watermark;
                QUAD_en_arr[q]				                = cfg[idx].QUAD_en;
                QUAD_cfg_arr[q]->m_inMapDepth               = cfg[idx].inMapDepth;
                QUAD_cfg_arr[q]->m_stride				    = cfg[idx].stride;
                QUAD_cfg_arr[q]->m_num_output_rows			= cfg[idx].num_output_rows;
                QUAD_cfg_arr[q]->m_num_output_cols			= cfg[idx].num_output_cols;
                QUAD_cfg_arr[q]->m_num_kernels				= cfg[idx].num_kernels;
                QUAD_cfg_arr[q]->m_master_QUAD				= cfg[idx].master_QUAD;
                QUAD_cfg_arr[q]->m_cascade				    = cfg[idx].cascade;
                QUAD_cfg_arr[q]->m_num_expd_input_rows      = cfg[idx].num_expd_input_rows;
                QUAD_cfg_arr[q]->m_num_expd_input_cols		= cfg[idx].num_expd_input_cols;
                QUAD_cfg_arr[q]->m_activation				= cfg[idx].activation;
                QUAD_cfg_arr[q]->m_padding				    = cfg[idx].padding;
                QUAD_cfg_arr[q]->m_upsample				    = cfg[idx].upsample;
                QUAD_cfg_arr[q]->m_crpd_input_row_start     = cfg[idx].crpd_input_row_start;
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

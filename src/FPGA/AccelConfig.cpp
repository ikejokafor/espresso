#include "AccelConfig.hpp"
using namespace std;
using namespace espresso;


AccelConfig::AccelConfig(FPGA_hndl* fpga_hndl) : Accel_Payload()
{
	m_fpga_hndl       = fpga_hndl;
    m_buffer          = NULL;
    m_size            = 0;
    m_remAddress      = -1;
}


AccelConfig::~AccelConfig()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    sysc_fpga_hndl->deallocate(this);
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
#else
    
#endif
}


void AccelConfig::serialize()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    printf("[ESPRESSO]: Allocating Space for Accelerator Configuration\n");
    m_size                          = MAX_AWP_PER_FAS * NUM_TOTAL_QUADS * sizeof(cfg_t);
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    
    cfg_t* cfg = (cfg_t*)m_buffer;
    for(int f = 0; f < NUM_FAS; f++)
    {       
        for (int a = 0; a < MAX_AWP_PER_FAS; a++)
        {
            auto& AWP_en_arr    = m_FAS_cfg_arr[f]->m_AWP_en_arr;
            auto& QUAD_cfg_arr  = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
            auto& QUAD_en_arr   = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
            for (int q = 0; q < NUM_TOTAL_QUADS; q++)
            {
                int idx 							= index2D(NUM_TOTAL_QUADS, a, q);
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
                cfg[idx].rm_ftch_vld_total          = m_FAS_cfg_arr[f]->m_rm_ftch_vld_total;
                cfg[idx].pm_ftch_vld_total          = m_FAS_cfg_arr[f]->m_pm_ftch_vld_total;
                cfg[idx].pv_ftch_vld_total          = m_FAS_cfg_arr[f]->m_pv_ftch_vld_total;
                cfg[idx].krnl1x1_pding				= m_FAS_cfg_arr[f]->m_krnl1x1_pding;
                cfg[idx].krnl1x1_pad_bgn			= m_FAS_cfg_arr[f]->m_krnl1x1_pad_bgn;
                cfg[idx].krnl1x1_pad_end			= m_FAS_cfg_arr[f]->m_krnl1x1_pad_end;
                cfg[idx].outMapStoreFactor          = m_FAS_cfg_arr[f]->m_outMapStoreFactor;
                cfg[idx].inMapAddr     			    = m_FAS_cfg_arr[f]->m_inMapAddr;
                cfg[idx].inMapFetchAmt              = m_FAS_cfg_arr[f]->m_inMapFetchAmt;                
                cfg[idx].krnl3x3Addr	 			= m_FAS_cfg_arr[f]->m_krnl3x3Addr;
                cfg[idx].krnl3x3BiasAddr			= m_FAS_cfg_arr[f]->m_krnl3x3BiasAddr;
                cfg[idx].partMapAddr				= m_FAS_cfg_arr[f]->m_partMapAddr;
                cfg[idx].resMapAddr					= m_FAS_cfg_arr[f]->m_resMapAddr;
                cfg[idx].outMapAddr					= m_FAS_cfg_arr[f]->m_outMapAddr;
                cfg[idx].prevMapAddr				= m_FAS_cfg_arr[f]->m_prevMapAddr;
                cfg[idx].pixelSeqAddr 				= m_FAS_cfg_arr[f]->m_pixelSeqAddr;
                cfg[idx].fas_num_output_rows		= m_FAS_cfg_arr[f]->m_num_output_rows;
                cfg[idx].fas_num_output_cols		= m_FAS_cfg_arr[f]->m_num_output_cols;
                cfg[idx].fas_outputh_depth          = m_FAS_cfg_arr[f]->m_output_depth;
                cfg[idx].act1x1                     = (uint64_t)m_FAS_cfg_arr[f]->m_act1x1;
                cfg[idx].it_act1x1                  = m_FAS_cfg_arr[f]->m_it_act1x1;
                cfg[idx].it_bias1x1                 = m_FAS_cfg_arr[f]->m_it_bias1x1;
                cfg[idx].om_store_vld_total         = m_FAS_cfg_arr[f]->m_om_store_vld_total;
                cfg[idx].kernel_i                   = m_FAS_cfg_arr[f]->m_kernel_i;
                cfg[idx].depth_i                    = m_FAS_cfg_arr[f]->m_depth_i;
                strcpy(cfg[idx].layerName, m_FAS_cfg_arr[f]->m_layerName.c_str());
                cfg[idx].last                       = m_FAS_cfg_arr[f]->m_last;
                // Serialize
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1BiasAddrArr =         
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1AddrArr =
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1No = 
                cfg[idx].AWP_en		                = AWP_en_arr[a];
                cfg[idx].FAS_id 					= QUAD_cfg_arr[q]->m_FAS_id;
                cfg[idx].AWP_id 					= QUAD_cfg_arr[q]->m_AWP_id;
                cfg[idx].QUAD_id 					= QUAD_cfg_arr[q]->m_QUAD_id;
                cfg[idx].res_high_watermark         = QUAD_cfg_arr[q]->m_res_high_watermark;
                cfg[idx].QUAD_en					= QUAD_en_arr[q];
                cfg[idx].inMapDepth                 = QUAD_cfg_arr[q]->m_inMapDepth;
                cfg[idx].stride 					= QUAD_cfg_arr[q]->m_stride;
                cfg[idx].quad_num_output_rows 		= QUAD_cfg_arr[q]->m_num_output_rows;
                cfg[idx].quad_num_output_cols	    = QUAD_cfg_arr[q]->m_num_output_cols;
                cfg[idx].num_kernels 				= QUAD_cfg_arr[q]->m_num_kernels;
                cfg[idx].master_QUAD 				= QUAD_cfg_arr[q]->m_master_QUAD;
                cfg[idx].cascade 					= QUAD_cfg_arr[q]->m_cascade;
                cfg[idx].num_expd_input_rows 		= QUAD_cfg_arr[q]->m_num_expd_input_rows;
                cfg[idx].num_expd_input_cols 		= QUAD_cfg_arr[q]->m_num_expd_input_cols;
                cfg[idx].num_input_rows             = QUAD_cfg_arr[q]->m_num_input_rows;
                cfg[idx].num_input_cols             = QUAD_cfg_arr[q]->m_num_input_cols;
                cfg[idx].kernel3x3Depth             = QUAD_cfg_arr[q]->m_kernel3x3Depth;
                cfg[idx].act3x3                     = (uint64_t)QUAD_cfg_arr[q]->m_act3x3;
                cfg[idx].it_act3x3                  = QUAD_cfg_arr[q]->m_it_act3x3;
                cfg[idx].it_bias3x3                 = QUAD_cfg_arr[q]->m_it_bias3x3;
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
    cfg_t* cfg = (cfg_t*)m_buffer;
    for(int f = 0; f < NUM_FAS; f++)
    {
        for (int a = 0; a < MAX_AWP_PER_FAS; a++)
        {
            auto& AWP_en_arr    = m_FAS_cfg_arr[f]->m_AWP_en_arr;
            auto& QUAD_cfg_arr  = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
            auto& QUAD_en_arr   = m_FAS_cfg_arr[f]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
            for (int q = 0; q < NUM_TOTAL_QUADS; q++)
            {
                int idx 								    = index2D(NUM_TOTAL_QUADS, a, q);
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
                m_FAS_cfg_arr[f]->m_outMapStoreFactor       = cfg[idx].outMapStoreFactor;
                m_FAS_cfg_arr[f]->m_co_high_watermark       = cfg[idx].co_high_watermark;
                m_FAS_cfg_arr[f]->m_rm_low_watermark        = cfg[idx].rm_low_watermark;
                m_FAS_cfg_arr[f]->m_pm_low_watermark        = cfg[idx].pm_low_watermark;
                m_FAS_cfg_arr[f]->m_pv_low_watermark        = cfg[idx].pv_low_watermark;
                m_FAS_cfg_arr[f]->m_rm_ftch_vld_total       = cfg[idx].rm_ftch_vld_total; 
                m_FAS_cfg_arr[f]->m_pm_ftch_vld_total       = cfg[idx].pm_ftch_vld_total; 
                m_FAS_cfg_arr[f]->m_pv_ftch_vld_total       = cfg[idx].pv_ftch_vld_total;
                m_FAS_cfg_arr[f]->m_krnl1x1_pding           = cfg[idx].krnl1x1_pding;
                m_FAS_cfg_arr[f]->m_krnl1x1_pad_bgn         = cfg[idx].krnl1x1_pad_bgn;
                m_FAS_cfg_arr[f]->m_krnl1x1_pad_end         = cfg[idx].krnl1x1_pad_end;
                m_FAS_cfg_arr[f]->m_act1x1                  = (activation_t)cfg[idx].act1x1;
                m_FAS_cfg_arr[f]->m_it_act1x1               = cfg[idx].it_act1x1;
                m_FAS_cfg_arr[f]->m_it_bias1x1              = cfg[idx].it_bias1x1;
                m_FAS_cfg_arr[f]->m_inMapAddr               = cfg[idx].inMapAddr;  	 
                m_FAS_cfg_arr[f]->m_inMapFetchAmt           = cfg[idx].inMapFetchAmt;         
                m_FAS_cfg_arr[f]->m_krnl3x3Addr             = cfg[idx].krnl3x3Addr;	 	
                m_FAS_cfg_arr[f]->m_krnl3x3BiasAddr         = cfg[idx].krnl3x3BiasAddr;	
                m_FAS_cfg_arr[f]->m_partMapAddr				= cfg[idx].partMapAddr;
                m_FAS_cfg_arr[f]->m_resMapAddr				= cfg[idx].resMapAddr;
                m_FAS_cfg_arr[f]->m_outMapAddr				= cfg[idx].outMapAddr;
                m_FAS_cfg_arr[f]->m_pixelSeqAddr			= cfg[idx].pixelSeqAddr;
                m_FAS_cfg_arr[f]->m_prevMapAddr				= cfg[idx].prevMapAddr;			
                m_FAS_cfg_arr[f]->m_num_output_rows			= cfg[idx].fas_num_output_rows;
                m_FAS_cfg_arr[f]->m_num_output_cols			= cfg[idx].fas_num_output_cols;
                m_FAS_cfg_arr[f]->m_output_depth            = cfg[idx].fas_outputh_depth;               
                m_FAS_cfg_arr[f]->m_om_store_vld_total      = cfg[idx].om_store_vld_total;    
                m_FAS_cfg_arr[f]->m_kernel_i                = cfg[idx].kernel_i;                          
                m_FAS_cfg_arr[f]->m_depth_i                 = cfg[idx].depth_i;                         
                m_FAS_cfg_arr[f]->m_layerName               = cfg[idx].layerName;
                m_FAS_cfg_arr[f]->m_last                    = cfg[idx].last;
                // Deserialize
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1BiasAddrArr =         
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1AddrArr =
                // m_FAS_cfg_arr[i]->m_itNKrnl1x1No = 
                AWP_en_arr[a]                               = cfg[idx].AWP_en;
                QUAD_cfg_arr[q]->m_FAS_id				    = cfg[idx].FAS_id;
                QUAD_cfg_arr[q]->m_AWP_id				    = cfg[idx].AWP_id;
                QUAD_cfg_arr[q]->m_QUAD_id				    = cfg[idx].QUAD_id;
                QUAD_cfg_arr[q]->m_res_high_watermark       = cfg[idx].res_high_watermark;
                QUAD_en_arr[q]				                = cfg[idx].QUAD_en;
                QUAD_cfg_arr[q]->m_inMapDepth               = cfg[idx].inMapDepth;
                QUAD_cfg_arr[q]->m_stride				    = cfg[idx].stride;
                QUAD_cfg_arr[q]->m_num_output_rows			= cfg[idx].quad_num_output_rows;
                QUAD_cfg_arr[q]->m_num_output_cols			= cfg[idx].quad_num_output_cols;
                QUAD_cfg_arr[q]->m_num_kernels				= cfg[idx].num_kernels;
                QUAD_cfg_arr[q]->m_master_QUAD				= cfg[idx].master_QUAD;
                QUAD_cfg_arr[q]->m_cascade				    = cfg[idx].cascade;
                QUAD_cfg_arr[q]->m_num_expd_input_rows      = cfg[idx].num_expd_input_rows;
                QUAD_cfg_arr[q]->m_num_expd_input_cols		= cfg[idx].num_expd_input_cols;
                QUAD_cfg_arr[q]->m_num_input_rows           = cfg[idx].num_input_rows;
                QUAD_cfg_arr[q]->m_num_input_cols           = cfg[idx].num_input_cols;
                QUAD_cfg_arr[q]->m_kernel3x3Depth           = cfg[idx].kernel3x3Depth;
                QUAD_cfg_arr[q]->m_act3x3                   = (activation_t)cfg[idx].act3x3;
                QUAD_cfg_arr[q]->m_it_act3x3                = cfg[idx].it_act3x3;
                QUAD_cfg_arr[q]->m_it_bias3x3               = cfg[idx].it_bias3x3;
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

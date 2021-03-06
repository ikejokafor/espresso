#pragma once


#include <vector>
#include <stdint.h>
#include "AWP_cfg.hpp"
#include "espresso_common.hpp"


typedef enum
{
    NULL_OPCODE,
	OPCODE_0	= 0,
    OPCODE_1    = 1, 
    OPCODE_2    = 2, 
    OPCODE_3    = 3, 
    OPCODE_4    = 4,
    OPCODE_5    = 5, 
    OPCODE_6    = 6, 
    OPCODE_7    = 7, 
    OPCODE_8    = 8, 
    OPCODE_9    = 9, 
    OPCODE_10   = 10,
    OPCODE_11   = 11,
    OPCODE_12   = 12,
    OPCODE_13   = 13,
    OPCODE_14   = 14,
    OPCODE_15   = 15,
    OPCODE_16   = 16,
    OPCODE_17   = 17
} opcode_t;


class FAS_cfg
{
    public:
        FAS_cfg();
        FAS_cfg(
            int FAS_id,
            opcode_t opcode,
            uint64_t pixelSeqAddr,
            uint64_t krnl1x1Addr,
            uint64_t krnl1x1BiasAddr,
            uint64_t partMapAddr,
            uint64_t resMapAddr,
			uint64_t prevMapAddr,
            uint64_t outMapAddr,
            int pixSeqCfgFetchTotal,
            int inMapFetchTotal,
            int krnl3x3FetchTotal,
            int krnl1x1FetchTotal,
            int krnl3x3BiasFetchTotal,
            int krnl1x1BiasFetchTotal,
            int num_1x1_kernels,
            int krnl1x1Depth,
            int partMapFetchTotal,
            int resMapFetchTotal,
            int outMapStoreTotal,
            int prevMapFetchTotal,
            int co_high_watermark,
            int rm_low_watermark,
            int pm_low_watermark,
            int pv_low_watermark,
            int rm_fetch_amount,
            int pm_fetch_amount,
            int pv_fetch_amount,
            bool krnl1x1_pding ,
            int krnl1x1_pad_bgn,
            int krnl1x1_pad_end,
            espresso::activation_t act1x1,
            bool it_act1x1,
            bool it_bias1x1,
			int num_output_rows,
			int num_output_cols,
			int output_depth,
            int om_store_vld_total,
            std::string layerName,
            int kernel_i,
            int depth_i,
            bool first,
            bool last
        );
        ~FAS_cfg();

        int m_FAS_id;
        opcode_t m_opcode;
        uint64_t m_krnl1x1Addr;
        uint64_t m_krnl1x1BiasAddr;
        uint64_t m_pixelSeqAddr;
        uint64_t m_partMapAddr;
        uint64_t m_resMapAddr;
        uint64_t m_outMapAddr;
		uint64_t m_prevMapAddr;
        uint64_t m_inMapAddr;
        uint64_t m_krnl3x3Addr;
        uint64_t m_krnl3x3BiasAddr;
        std::vector<uint64_t> m_itNKrnl1x1BiasAddrArr;        
        std::vector<uint64_t> m_itNKrnl1x1AddrArr;
        std::vector<uint64_t> m_itNKrnl1x1No;
        int m_pixSeqCfgFetchTotal;
        int m_inMapFetchTotal;
        int m_krnl3x3FetchTotal;
        int m_krnl3x3BiasFetchTotal;
        int m_krnl1x1FetchTotal;
        int m_krnl1x1BiasFetchTotal;
        int m_num_1x1_kernels;
        int m_krnl1x1Depth;
        int m_partMapFetchTotal;
        int m_resMapFetchTotal;
        int m_outMapStoreTotal;
        int m_inMapFetchAmt;
        int m_outMapStoreFactor;
        int m_prevMapFetchTotal;
        int m_co_high_watermark;
        int m_rm_low_watermark;
        int m_pm_low_watermark;
        int m_pv_low_watermark;
        int m_rm_ftch_vld_total;
        int m_pm_ftch_vld_total;
        int m_pv_ftch_vld_total;
        bool m_krnl1x1_pding;
        int m_krnl1x1_pad_bgn;
        int m_krnl1x1_pad_end;
        espresso::activation_t m_act1x1;
        bool m_it_act1x1;
        bool m_it_bias1x1;
		int m_num_output_rows;
		int m_num_output_cols;
		int m_output_depth;
        int m_om_store_vld_total;
        std::string m_layerName;
        int m_kernel_i;
        int m_depth_i;
        bool m_first;
        bool m_last;
        std::vector<AWP_cfg*> m_AWP_cfg_arr;
        std::vector<bool> m_AWP_en_arr;
        int m_FGPA_VEC_ADD_SIMD;    // read from FPGA cfg
};

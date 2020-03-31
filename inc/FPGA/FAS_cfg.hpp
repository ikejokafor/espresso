#pragma once


#include <vector>
#include <stdint.h>
#include "AWP_cfg.hpp"


class FAS_cfg
{ 
    public:
		FAS_cfg(
				int FAS_id, 
				bool do_kernel1x1, 
				bool do_res_layer, 
				bool first_depth_iter, 
				bool last_depth_iter,
				int inMapFetchTotal,
				int partMapFetchTotal,
				int krnl1x1FetchTotal,
				int krnl3x3FetchTotal,
				int resMapFetchTotal,
				int outMapStoreTotal
		);
        ~FAS_cfg();
	
		bool m_do_res_layer;
		bool m_first_depth_iter;
		bool m_last_depth_iter;
        int m_FAS_id;
		bool m_do_kernel1x1;
		uint64_t m_imAddrArr[MAX_QUAD_PER_AWP];
		uint64_t m_krnl3x3AddrArr[MAX_QUAD_PER_AWP];
        uint64_t m_krnl1x1AddrArr;
        uint64_t m_biasAddr;
        uint64_t m_pixelSeqAddr;
		uint64_t m_partMapAddr;
		uint64_t m_resMapAddr;
		uint64_t m_outMapAddr;
	    std::vector<AWP_cfg*> m_AWP_cfg_arr;
		std::vector<bool> m_AWP_en_arr;
		int	m_partMapFetchTotal     ;
		int	m_inMapFetchTotal       ;
		int	m_krnl1x1FetchTotal     ;
		int m_krnl3x3FetchTotal		;	
		int	m_resMapFetchTotal      ;
		int m_outMapStoreTotal		;
};

#pragma once


#include <vector>
#include <stdint.h>
#include "AWP_cfg.hpp"


class FAS_cfg
{ 
    public:
		FAS_cfg();
		FAS_cfg(
			int FAS_id, 
			bool do_kernels1x1, 
			bool do_res_layer, 
			bool first_depth_iter, 
			bool last_depth_iter,
			uint64_t pixelSeqAddr,
			uint64_t krnl1x1BiasAddr,
			uint64_t partMapAddr,
			uint64_t resMapAddr,
			uint64_t outMapAddr,
			int pixSeqCfgFetchTotal,			
			int inMapFetchTotal,
			int krnl3x3FetchTotal,
			int krnl1x1FetchTotal,
			int krnl3x3BiasFetchTotal,
			int krnl1x1BiasFetchTotal,
			int partMapFetchTotal,
			int resMapFetchTotal,
			int outMapStoreTotal
		);
        ~FAS_cfg();
	
	    int m_FAS_id;
		bool m_do_kernels1x1;
		bool m_do_res_layer;
		bool m_first_depth_iter;
		bool m_last_depth_iter;
        uint64_t m_krnl1x1BiasAddr;
        uint64_t m_pixelSeqAddr;
		uint64_t m_partMapAddr;
		uint64_t m_resMapAddr;
		uint64_t m_outMapAddr;
		int m_pixSeqCfgFetchTotal;
		int	m_inMapFetchTotal;
		int m_krnl3x3FetchTotal;
		int m_krnl3x3BiasFetchTotal;		
		int	m_krnl1x1FetchTotal;
		int m_krnl1x1BiasFetchTotal;
		int	m_partMapFetchTotal;
		int	m_resMapFetchTotal;
		int m_outMapStoreTotal;
	    std::vector<AWP_cfg*> m_AWP_cfg_arr;
		std::vector<bool> m_AWP_en_arr;
};

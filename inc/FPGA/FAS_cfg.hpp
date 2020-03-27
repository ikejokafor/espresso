#pragma once


#include <vector>
#include <stdint.h>
#include "AWP_cfg.hpp"


class FAS_cfg
{ 
    public:
		FAS_cfg(int FAS_id, bool kernel_1x1, bool res_layer, bool first_iter, bool last_iter);
        ~FAS_cfg();
	
		bool m_res_layer;
		bool m_first_iter;
		bool m_last_iter;
        int m_FAS_id;
		bool m_kernel_1x1;
		uint64_t imAddrArr[MAX_QUAD_PER_AWP];
        uint64_t krnl3x3AddrArr;
        uint64_t krnl1x1AddrArr;
        uint64_t biasAddr;
        uint64_t pixelSeqAddr;
	    std::vector<AWP_cfg*> m_AWP_cfg_arr;
		std::vector<bool> m_AWP_en_arr;
		int	m_partMapFetchTotal     ;
		int	m_inMapFetchTotal       ;
		int	m_krnlFetchTotal        ;
		int	m_resMapFetchTotal      ;
};

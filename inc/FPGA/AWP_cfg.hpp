#pragma once


#include <vector>
#include "common.hpp"
#include "QUAD_cfg.hpp"


class AWP_cfg
{
    public:
	    AWP_cfg(int FAS_id, int AWP_id);
        ~AWP_cfg();
        
        int m_FAS_id;
        int m_AWP_id;
	    std::vector<QUAD_cfg*> m_AWP_QUAD_cfg_arr;
	    std::vector<bool> m_AWP_QUAD_cfg_en_arr;
};

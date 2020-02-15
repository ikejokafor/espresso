#pragma once


#include <vector>
#include "QUAD_cfg.hpp"


class AWP_cfg
{
    public:
	    AWP_cfg(int FAS_id, int AWP_id);
        ~AWP_cfg();
        
        int m_FAS_id;
        int m_AWP_id;
	    std::vector<QUAD_cfg*> m_quad_cfg_arr;
};

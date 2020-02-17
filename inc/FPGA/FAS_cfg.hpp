#pragma once


#include <vector>
#include "AWP_cfg.hpp"


class FAS_cfg
{
    public:
		FAS_cfg(int FAS_id, bool residual);
        ~FAS_cfg();
	
		bool m_residual;
        int m_FAS_id;
	    std::vector<AWP_cfg*> m_awp_cfg_arr;
};

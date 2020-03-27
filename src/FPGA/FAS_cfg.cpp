#include "FAS_cfg.hpp"


FAS_cfg::FAS_cfg(int FAS_id, bool kernel_1x1, bool res_layer, bool first_iter, bool last_iter)
{
	m_FAS_id = FAS_id;
	m_res_layer = res_layer;
	m_first_iter = first_iter;
	m_last_iter = last_iter;
	m_kernel_1x1 = kernel_1x1;
}


FAS_cfg::~FAS_cfg()
{
    
}

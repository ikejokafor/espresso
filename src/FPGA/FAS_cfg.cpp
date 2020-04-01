#include "FAS_cfg.hpp"


FAS_cfg::FAS_cfg() { }


FAS_cfg::FAS_cfg(
	int FAS_id, 
	bool do_kernel1x1, 
	bool res_layer, 
	bool first_depth_iter, 
	bool last_depth_iter,
	int inMapFetchTotal,
	int partMapFetchTotal,
	int krnl1x1FetchTotal,
	int krnl3x3FetchTotal,
	int resMapFetchTotal,
	int outMapStoreTotal
) {
	m_FAS_id = FAS_id;
	m_do_res_layer = res_layer;
	m_first_depth_iter = first_depth_iter;
	m_last_depth_iter = last_depth_iter;
	m_do_kernel1x1 = do_kernel1x1;
	m_inMapFetchTotal = inMapFetchTotal;	
	m_partMapFetchTotal = partMapFetchTotal;
	m_krnl1x1FetchTotal = krnl1x1FetchTotal;
	m_krnl3x3FetchTotal = krnl3x3FetchTotal;
	m_resMapFetchTotal = resMapFetchTotal;
	m_outMapStoreTotal = outMapStoreTotal;
}


FAS_cfg::~FAS_cfg()
{
    
}

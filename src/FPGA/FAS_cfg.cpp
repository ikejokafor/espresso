#include "FAS_cfg.hpp"
using namespace std;


FAS_cfg::FAS_cfg() :
   	m_inMapAddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP)),
	m_krnl3x3AddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP)),
	m_krnl3x3BiasAddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP))
{ }


FAS_cfg::FAS_cfg(
	int FAS_id,
	bool do_kernels1x1,
	bool do_res_layer,
	bool first_depth_iter,
	uint64_t pixelSeqAddr,
	uint64_t krnl1x1Addr,
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
	int num_1x1_kernels,
	int krnl1x1Depth,
	int partMapFetchTotal,
	int resMapFetchTotal,
	int outMapStoreTotal
) :
   	m_inMapAddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP)),
	m_krnl3x3AddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP)),
	m_krnl3x3BiasAddrArr(MAX_AWP_PER_FAS, vector<uint64_t>(MAX_QUAD_PER_AWP))
{
	m_FAS_id = FAS_id;
	m_do_kernels1x1 = do_kernels1x1;
	m_do_res_layer = do_res_layer;
	m_first_depth_iter = first_depth_iter;
	m_krnl1x1Addr = krnl1x1Addr;
	m_krnl1x1BiasAddr = krnl1x1BiasAddr;
	m_pixelSeqAddr = pixelSeqAddr;
	m_partMapAddr = partMapAddr;
	m_resMapAddr = resMapAddr;
	m_outMapAddr = outMapAddr;
	m_pixSeqCfgFetchTotal = pixSeqCfgFetchTotal;
	m_inMapFetchTotal = inMapFetchTotal;
	m_krnl3x3FetchTotal = krnl3x3FetchTotal;
	m_krnl1x1FetchTotal = krnl1x1FetchTotal;
	m_krnl3x3BiasFetchTotal = krnl3x3BiasFetchTotal;
	m_krnl1x1BiasFetchTotal = krnl1x1BiasFetchTotal;
	m_num_1x1_kernels = num_1x1_kernels;
	m_krnl1x1Depth = krnl1x1Depth;
	m_partMapFetchTotal = partMapFetchTotal;
	m_resMapFetchTotal = resMapFetchTotal;
	m_outMapStoreTotal = outMapStoreTotal;
}


FAS_cfg::~FAS_cfg()
{

}

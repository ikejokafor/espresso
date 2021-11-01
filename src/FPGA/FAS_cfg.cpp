#include "FAS_cfg.hpp"
using namespace std;
using namespace espresso;


FAS_cfg::FAS_cfg() :
   	m_itNKrnl1x1BiasAddrArr(MAX_FAS_1X1_IT),
	m_itNKrnl1x1AddrArr(MAX_FAS_1X1_IT),
    m_itNKrnl1x1No(MAX_FAS_1X1_IT)
{ }


FAS_cfg::FAS_cfg(
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
	int rm_ftch_vld_total,
	int pm_ftch_vld_total,
	int pv_ftch_vld_total,
	bool krnl1x1_pding,
	int krnl1x1_pad_bgn,
	int krnl1x1_pad_end,
    activation_t act1x1,
    bool it_act1x1,
    bool it_bias1x1,
	int num_output_rows,
	int num_output_cols,
	int output_depth,
    int om_store_vld_total
#ifdef SYSTEMC
    ,
    string layerName,
    int kernel_i,
    int depth_i,
    bool first,
    bool last
#endif
) :
   	m_itNKrnl1x1BiasAddrArr(MAX_FAS_1X1_IT),
	m_itNKrnl1x1AddrArr(MAX_FAS_1X1_IT),
    m_itNKrnl1x1No(MAX_FAS_1X1_IT)
{
	m_opcode = opcode;
	m_FAS_id = FAS_id;
	m_krnl1x1Addr = krnl1x1Addr;
	m_krnl1x1BiasAddr = krnl1x1BiasAddr;
	m_pixelSeqAddr = pixelSeqAddr;
	m_partMapAddr = partMapAddr;
	m_resMapAddr = resMapAddr;
	m_prevMapAddr = prevMapAddr;
	m_outMapAddr = outMapAddr;
	m_pixSeqCfgFetchTotal = pixSeqCfgFetchTotal;
	m_inMapFetchTotal = inMapFetchTotal;
	m_krnl3x3FetchTotal = krnl3x3FetchTotal;
	m_krnl1x1FetchTotal = krnl1x1FetchTotal;
	m_krnl3x3BiasFetchTotal = krnl3x3BiasFetchTotal;
	m_krnl1x1BiasFetchTotal = krnl1x1BiasFetchTotal;
	m_num_1x1_kernels = num_1x1_kernels;
	m_partMapFetchTotal = partMapFetchTotal;
	m_resMapFetchTotal = resMapFetchTotal;
	m_outMapStoreTotal = outMapStoreTotal;
    m_prevMapFetchTotal = prevMapFetchTotal;
	m_co_high_watermark = co_high_watermark;
    m_rm_low_watermark = rm_low_watermark;
    m_pm_low_watermark = pm_low_watermark;
    m_pv_low_watermark = pv_low_watermark;
	m_rm_ftch_vld_total = rm_ftch_vld_total;
	m_pm_ftch_vld_total = pm_ftch_vld_total;
	m_pv_ftch_vld_total = pv_ftch_vld_total;
	m_krnl1x1_pding = krnl1x1_pding;
	m_krnl1x1_pad_bgn = krnl1x1_pad_bgn;
	m_krnl1x1_pad_end = krnl1x1_pad_end;
    m_act1x1 = act1x1;
    m_it_act1x1 = it_act1x1;
    m_it_bias1x1 = it_bias1x1;
	m_num_output_rows = num_output_rows;
	m_num_output_cols = num_output_cols;
	m_output_depth = output_depth;
    m_om_store_vld_total = om_store_vld_total;
#ifdef SYSTEMC
	m_krnl1x1Depth = krnl1x1Depth;
    m_layerName = layerName;
    m_kernel_i = kernel_i;
    m_depth_i = depth_i;
    m_first = first;
    m_last = last;
#else
    m_krnl1x1Depth = krnl1x1Depth / m_FGPA_VEC_ADD_SIMD;
#endif
}


FAS_cfg::~FAS_cfg()
{

}

#include "Layer_Iteration.hpp"
using namespace std;
using namespace espresso;


Layer_Iteration::Layer_Iteration(
	FPGA_hndl* fpga_hndl,
	opcode_t opcode,
	InputMaps* inputMaps,
	Kernels* kernels3x3,
	Kernels* kernels1x1,
	KernelBias* kernels3x3Bias,
	KernelBias* kernels1x1Bias,
	PartialMaps* partialMaps,
	ResidualMaps* residualMaps,
	OutputMaps* outputMaps,
    Prev1x1Maps* prev1x1Maps,
	int stride,
	bool upsample,
	int padding,
    int group,
    activation_t act3x3,
    activation_t act1x1,
    bool it_act3x3,
    bool it_act1x1,
    bool it_bias3x3,
    bool it_bias1x1,
	bool krnl1x1_pding,
	int krnl1x1_pad_bgn,
	int krnl1x1_pad_end,   
    string layerName,
    int kernel_i,
    int depth_i,
    bool first,
    bool last
    )
{
    m_layerName         = layerName;
	m_opcode			= opcode;
	m_pxSeqCfg			= NULL;
	m_inputMaps 		= NULL;
	m_kernels3x3		= NULL;
	m_residualMaps		= NULL;
	m_outputMaps		= NULL;
	m_kernels3x3Bias	= NULL;
	m_kernels1x1		= NULL;
	m_kernels1x1Bias	= NULL;
	m_partialMaps       = NULL;
	m_prev1x1Maps		= NULL;
	m_pxSeqCfg = new PixelSeqCfg(fpga_hndl, stride);
	m_accelCfg = new AccelConfig(fpga_hndl);
	m_inputMaps	= inputMaps;
	m_kernels3x3 = kernels3x3;
	m_kernels1x1 = kernels1x1;
	m_kernels3x3Bias = kernels3x3Bias;
	m_kernels1x1Bias = kernels1x1Bias;
	m_partialMaps = partialMaps;
	m_residualMaps = residualMaps;
	m_outputMaps = outputMaps;
	m_prev1x1Maps = prev1x1Maps;
    m_krnl1x1_pding = krnl1x1_pding;
    m_krnl1x1_pad_bgn = krnl1x1_pad_bgn;
    m_krnl1x1_pad_end = krnl1x1_pad_end;
    m_act1x1 = act1x1;
    m_act3x3 = act3x3;
    m_it_act1x1 = it_act1x1;
    m_it_act3x3 = it_act3x3;
    m_it_bias1x1 = it_bias1x1;
    m_it_bias3x3 = it_bias3x3;
    m_upsample = upsample;      
    m_kernel_i = kernel_i;
    m_depth_i = depth_i;
    m_first = first;
    m_last = last;
    m_stride = stride; 
    m_padding = padding;
    m_group = group;
}


Layer_Iteration::~Layer_Iteration()
{
	(m_prev1x1Maps) ? delete m_prev1x1Maps : void();
	(m_pxSeqCfg) ? delete m_pxSeqCfg : void();
	(m_accelCfg) ? delete m_accelCfg : void();
	(m_inputMaps) ? delete m_inputMaps : void();
	(m_kernels3x3) ? delete m_kernels3x3 : void();
	(m_residualMaps) ? delete m_residualMaps : void();
	(m_outputMaps) ? delete m_outputMaps : void();
	(m_kernels3x3Bias) ? delete m_kernels3x3Bias : void();
    (m_kernels1x1) ? delete m_kernels1x1 : void();
	(m_partialMaps) ? delete m_partialMaps : void();	
	m_prev1x1Maps = NULL;
    m_pxSeqCfg = NULL;
	m_accelCfg = NULL;
	m_inputMaps = NULL;
	m_kernels3x3 = NULL;
	m_residualMaps = NULL;
	m_outputMaps = NULL;
	m_kernels3x3Bias = NULL;
    m_kernels1x1 = NULL;
    m_kernels1x1Bias = NULL;
	m_partialMaps = NULL;
}


void Layer_Iteration::prepFPGAData()
{
	(m_inputMaps) ? m_inputMaps->serialize() : void();
	(m_kernels3x3) ? m_kernels3x3->serialize() : void();
	(m_kernels3x3Bias) ? m_kernels3x3Bias->serialize() : void();
    m_outputMaps->serialize();
	(m_kernels1x1) ? m_kernels1x1->serialize() : void();
	(m_kernels1x1Bias) ? m_kernels1x1Bias->serialize() : void();
	(m_partialMaps) ? m_partialMaps->serialize() : void();
	(m_residualMaps) ? m_residualMaps->serialize() : void();
	(m_prev1x1Maps) ? m_prev1x1Maps->serialize() : void();


    int inputMapDepth = (m_inputMaps) ? m_inputMaps->m_depth : 0;
	int remDepth = (m_inputMaps) ? m_inputMaps->m_depth : 0;
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(
			i,
			m_opcode,
			m_pxSeqCfg->m_remAddress,
			(m_kernels1x1) ? m_kernels1x1->m_remAddress : -1,
			(m_kernels1x1Bias) ? m_kernels1x1Bias->m_remAddress : -1,
			(m_partialMaps) ? m_partialMaps->m_remAddress : -1,
			(m_residualMaps) ? m_residualMaps->m_remAddress : -1,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_remAddress : -1,
			m_outputMaps->m_remAddress,
			m_pxSeqCfg->m_size,
			(m_inputMaps) ? m_inputMaps->m_size : 0,
			(m_kernels3x3) ? m_kernels3x3->m_size : 0,
			(m_kernels1x1) ? m_kernels1x1->m_size : 0,
			(m_kernels3x3Bias) ? m_kernels3x3Bias->m_size : 0,
			(m_kernels1x1Bias) ? m_kernels1x1Bias->m_size : 0,
			(m_kernels1x1) ? m_kernels1x1->m_numKernels : 0,
			(m_kernels1x1) ? m_kernels1x1->m_depth : 0,
			(m_partialMaps) ? m_partialMaps->m_size : 0,
			(m_residualMaps) ? m_residualMaps->m_size : 0,
			m_outputMaps->m_size,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_size : 0,
			(m_kernels1x1) ? (m_kernels1x1->m_depth * CO_HIGH_WATERMARK_FACTOR) : (m_outputMaps->m_depth * CO_HIGH_WATERMARK_FACTOR),
			(m_residualMaps) ? m_residualMaps->m_ftchFctr : 0,
			(m_partialMaps) ? m_partialMaps->m_ftchFctr : 0,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_ftchFctr : 0,
			(m_residualMaps) ? m_residualMaps->m_vld_sz : 0,
			(m_partialMaps) ? m_partialMaps->m_vld_sz : 0,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_vld_sz : 0,
			m_krnl1x1_pding,
			m_krnl1x1_pad_bgn,
			m_krnl1x1_pad_end,
            m_act1x1,
            m_it_act1x1,
            m_it_bias1x1,
			m_outputMaps->m_rows,
			m_outputMaps->m_cols,
			m_outputMaps->m_depth,
            m_outputMaps->m_vld_sz,
            m_layerName,
            m_kernel_i,
            m_depth_i,
            m_first,
            m_last      
		));
		m_accelCfg->m_FAS_cfg_arr[i]->m_partMapAddr = (m_partialMaps) ? m_partialMaps->m_remAddress : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_resMapAddr = (m_residualMaps) ? m_residualMaps->m_remAddress : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapAddr = m_outputMaps->m_remAddress;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapStoreFactor = m_outputMaps->m_strFctr;
		m_accelCfg->m_FAS_cfg_arr[i]->m_inMapAddr = (m_inputMaps) ? m_inputMaps->m_remAddress : 0;
		m_accelCfg->m_FAS_cfg_arr[i]->m_inMapFetchAmt = (m_inputMaps) ? (m_inputMaps->m_cols_algnd * m_inputMaps->m_depth * PIXEL_SIZE) >> (int)log2(AXI_MX_BT_SZ) : 0;
        m_accelCfg->m_FAS_cfg_arr[i]->m_krnl3x3Addr = (m_kernels3x3) ? m_kernels3x3->m_remAddress : 0;
        m_accelCfg->m_FAS_cfg_arr[i]->m_krnl3x3BiasAddr = (m_kernels3x3Bias) ? m_kernels3x3Bias->m_remAddress : 0;
        // m_accelCfg->m_FAS_cfg_arr[i]->m_itNKrnl1x1BiasAddrArr =         
        // m_accelCfg->m_FAS_cfg_arr[i]->m_itNKrnl1x1AddrArr = 
        // m_accelCfg->m_FAS_cfg_arr[i]->m_itNKrnl1x1No = 
        for(int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr.push_back(new AWP_cfg(i ,j));
			for(int k = 0; k < MAX_QUAD_PER_AWP; k++)
			{
				auto& QUAD_cfg_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr;
				auto& QUAD_en_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_en_arr;
				if(remDepth > 0)
				{
					bool cascade = (remDepth > 0 && inputMapDepth > QUAD_MAX_DEPTH) ? true : false;
					bool master_QUAD = ((remDepth - QUAD_MAX_DEPTH) <= 0) ? true : false;
                    int quad_proc_depth = (remDepth > QUAD_MAX_DEPTH) ? QUAD_MAX_DEPTH : remDepth;
					QUAD_cfg* quad_cfg = new QUAD_cfg(
						i,
						j,
						k,
						true,
						m_inputMaps->m_rows,
						m_inputMaps->m_cols,
						(m_kernels3x3) ? m_kernels3x3->m_numKernels : 0,
						(m_kernels3x3) ? m_kernels3x3->m_depth : 0,
						(m_kernels3x3) ? m_kernels3x3->m_rows : 0,
						(m_kernels3x3) ? m_kernels3x3->m_cols : 0,
						m_stride,
						m_upsample,
						m_padding,
						m_act3x3,
                        (master_QUAD) ? m_it_act3x3 : false,
                        (master_QUAD) ? m_it_bias3x3 : false,
						master_QUAD,
						cascade,
						quad_proc_depth,
                        RE_HIGH_WATERMARK,
                        m_group
					);
					QUAD_cfg_arr.push_back(quad_cfg);
					QUAD_en_arr.push_back(true);
				}
				else
				{
					QUAD_cfg_arr.push_back(new QUAD_cfg(i, j, k, false));
					QUAD_en_arr.push_back(false);
				}
				remDepth -= QUAD_MAX_DEPTH;
			}
			m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_en_arr.push_back(true);
		}
	}
	m_accelCfg->serialize();
    // cout << "FIXME " << __FILE__ << ":" << __LINE__ << endl;
}
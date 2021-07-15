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
    activation_t act3x3,
    activation_t act1x1,
    bool it_act3x3,
    bool it_act1x1,
    bool it_bias3x3,
    bool it_bias1x1,
	bool krnl1x1_pding,
	int krnl1x1_pad_bgn,
	int krnl1x1_pad_end,
	bool del_res,
	bool del_1x1)
{
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
	m_del_res = del_res;
	m_del_1x1 = del_1x1;


	(m_inputMaps) ? m_inputMaps->serialize() : void();
	(m_kernels3x3) ? m_kernels3x3->serialize() : void();
	(m_kernels3x3Bias) ? m_kernels3x3Bias->serialize() : void();
    m_outputMaps->serialize();
	(m_kernels1x1) ? m_kernels1x1->serialize() : void();
	(m_kernels1x1Bias) ? m_kernels1x1Bias->serialize() : void();
	(m_partialMaps) ? m_partialMaps->serialize() : void();
	(m_residualMaps) ? m_residualMaps->serialize() : void();
	(m_prev1x1Maps) ? m_prev1x1Maps->serialize() : void();

    int inputMapDepth = (m_inputMaps) ? m_inputMaps->m_inputMapDepth : 0;
	int remDepth = (m_inputMaps) ? m_inputMaps->m_inputMapDepth : 0;
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(
			i,
			opcode,
			m_pxSeqCfg->m_remAddress,
			(m_kernels1x1) ? m_kernels1x1->m_remAddress : -1,
			(m_kernels1x1Bias) ? m_kernels1x1Bias->m_remAddress : -1,
			(m_partialMaps) ? m_partialMaps->m_remAddress : -1,
			(m_residualMaps) ? m_residualMaps->m_remAddress : -1,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_remAddress : -1,
			outputMaps->m_remAddress,
			m_pxSeqCfg->m_size,
			(m_inputMaps) ? m_inputMaps->m_size : 0,
			(m_kernels3x3) ? m_kernels3x3->m_size : 0,
			(m_kernels1x1) ? m_kernels1x1->m_size : 0,
			(m_kernels3x3Bias) ? m_kernels3x3Bias->m_size : 0,
			(m_kernels1x1Bias) ? m_kernels1x1Bias->m_size : 0,
			(m_kernels1x1) ? m_kernels1x1->m_numKernels : 0,
			(m_kernels1x1) ? m_kernels1x1->m_kernelDepth : 0,
			(m_partialMaps) ? m_partialMaps->m_size : 0,
			(m_residualMaps) ? m_residualMaps->m_size : 0,
			m_outputMaps->m_size,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_size : 0,
			(m_kernels1x1) ? (m_kernels1x1->m_kernelDepth * CO_HIGH_WATERMARK_FACTOR) : (m_outputMaps->m_outputMapDepth * CO_HIGH_WATERMARK_FACTOR),
			(m_residualMaps) ? m_residualMaps->m_residualMapDepth * RM_LOW_WATERMARK_FACTOR : 0,
			(m_partialMaps) ? m_partialMaps->m_partialMapDepth * PM_LOW_WATERMARK_FACTOR : 0,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_prev1x1MapDepth * PV_LOW_WATERMARK_FACTOR : 0,
			(m_residualMaps) ? m_residualMaps->m_residualMapDepth * RM_FETCH_FACTOR : 0,
			(m_partialMaps) ? m_partialMaps->m_partialMapDepth * PM_FETCH_FACTOR : 0,
            (m_prev1x1Maps) ? m_prev1x1Maps->m_prev1x1MapDepth * PV_FETCH_FACTOR : 0,
			krnl1x1_pding,
			krnl1x1_pad_bgn,
			krnl1x1_pad_end,
            act1x1,
            it_act1x1,
            it_bias1x1,
			m_outputMaps->m_numOutputMapRows,
			m_outputMaps->m_numOutputMapCols,
			m_outputMaps->m_outputMapDepth
		));
		m_accelCfg->m_FAS_cfg_arr[i]->m_partMapAddr = (m_partialMaps) ? m_partialMaps->m_remAddress : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_resMapAddr = (m_residualMaps) ? m_residualMaps->m_remAddress : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapAddr = m_outputMaps->m_remAddress;
		m_accelCfg->m_FAS_cfg_arr[i]->m_inMapFetchFactor = (m_inputMaps) ? m_inputMaps->m_numInputMapCols : 0;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapStoreFactor = m_outputMaps->m_outputMapDepth;
		auto& imAddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_inMapAddrArr;
		auto& krnl3x3AddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_krnl3x3AddrArr;
		auto& krnl3x3BiasAddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_krnl3x3BiasAddrArr;
		
        
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
					QUAD_cfg* quad_cfg = new QUAD_cfg(
						i,
						j,
						k,
						true,
						m_inputMaps->m_numInputMapRows,
						m_inputMaps->m_numInputMapCols,
						(m_kernels3x3) ? m_kernels3x3->m_numKernels : 0,
						(m_kernels3x3) ? m_kernels3x3->m_kernelDepth : 0,
						(m_kernels3x3) ? m_kernels3x3->m_numKernelRows : 0,
						(m_kernels3x3) ? m_kernels3x3->m_numKernelCols : 0,
						stride,
						upsample,
						padding,
						act3x3,
                        (master_QUAD) ? it_act3x3 : false,
                        (master_QUAD) ? it_bias3x3 : false,
						master_QUAD,
						cascade,
						(remDepth > QUAD_MAX_DEPTH) ? QUAD_MAX_DEPTH : remDepth
#ifdef SYSTEMC  
                        ,
                        RE_HIGH_WATERMARK
#endif
					);
					QUAD_cfg_arr.push_back(quad_cfg);
					QUAD_en_arr.push_back(true);
					int imDepthStep = QUAD_MAX_DEPTH * m_inputMaps->m_numInputMapRows * m_inputMaps->m_numInputMapCols;
					int krn3x3DepthStep = QUAD_MAX_DEPTH * 3 * 3;
					imAddrArr[j][k] = m_inputMaps->m_remAddress + (k * imDepthStep);
					krnl3x3AddrArr[j][k] = (kernels3x3) ? m_kernels3x3->m_remAddress + (k * krn3x3DepthStep) : 0;
					krnl3x3BiasAddrArr[j][k] = (kernels3x3Bias) ? m_kernels3x3Bias->m_remAddress : 0;
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
}


Layer_Iteration::~Layer_Iteration()
{
	(m_prev1x1Maps) ? delete m_prev1x1Maps : void();
	(m_pxSeqCfg) ? delete m_pxSeqCfg : void();
	(m_accelCfg) ? delete m_accelCfg : void();
	(m_inputMaps) ? delete m_inputMaps : void();
	(m_kernels3x3) ? delete m_kernels3x3 : void();
	(m_residualMaps && m_del_res) ? delete m_residualMaps : void();
	(m_outputMaps) ? delete m_outputMaps : void();
	(m_kernels3x3Bias) ? delete m_kernels3x3Bias : void();
    (m_kernels1x1 && m_del_1x1) ? delete m_kernels1x1 : void();
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
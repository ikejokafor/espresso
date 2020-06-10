#include "Layer_Iteration.hpp"
using namespace std;


Layer_Iteration::Layer_Iteration(
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
	bool activation,
	bool krnl1x1_pding,
	int krnl1x1_pad_bgn,
	int krnl1x1_pad_end) 
{
	m_opcode			= opcode;
	m_pxSeqCfg			= nullptr;
	m_inputMaps 		= nullptr;
	m_kernels3x3		= nullptr;
	m_residualMaps		= nullptr;
	m_outputMaps		= nullptr;
	m_kernels3x3Bias	= nullptr;
	m_kernels1x1		= nullptr;
	m_kernels1x1Bias	= nullptr;
	m_partialMaps       = nullptr;
	m_prev1x1Maps		= nullptr;
	m_pxSeqCfg = new PixelSeqCfg(stride);
	m_accelCfg = new AccelConfig();
	m_inputMaps	= inputMaps;
	m_kernels3x3 = kernels3x3;
	m_kernels1x1 = kernels1x1;
	m_kernels3x3Bias = kernels3x3Bias;
	m_kernels1x1Bias = kernels1x1Bias;
	m_partialMaps = partialMaps;
	m_residualMaps = residualMaps;
	m_outputMaps = outputMaps;
	m_prev1x1Maps = prev1x1Maps;


	m_inputMaps->serialize();
	(m_kernels3x3) ? m_kernels3x3->serialize() : void();
	(m_kernels3x3Bias) ? m_kernels3x3Bias->serialize() : void();
	m_outputMaps->serialize();
	(m_kernels1x1) ? m_kernels1x1->serialize() : void();
	(m_kernels1x1Bias) ? m_kernels1x1Bias->serialize() : void();
	(m_partialMaps) ? m_partialMaps->serialize() : void();
	(m_residualMaps) ? m_residualMaps->serialize() : void();
	(m_prev1x1Maps) ? m_prev1x1Maps->serialize() : void();

	int remDepth = inputMaps->m_inputMapDepth;
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(
			i,
			opcode,
			m_pxSeqCfg->m_address,
			(kernels1x1) ? kernels1x1->m_address : 0,
			(kernels1x1Bias) ? kernels1x1Bias->m_address : 0,
			(partialMaps) ? partialMaps->m_address : 0,
			(residualMaps) ? residualMaps->m_address : 0,
			outputMaps->m_address,
			m_pxSeqCfg->m_size,
			(inputMaps) ? inputMaps->m_size : 0,
			(kernels3x3) ? kernels3x3->m_size : 0,
			(kernels1x1) ? kernels1x1->m_size : 0,
			(kernels3x3Bias) ? kernels3x3Bias->m_size : 0,
			(kernels1x1Bias) ? kernels1x1Bias->m_size : 0,
			(kernels1x1) ? kernels1x1->m_numKernels : 0,
			(kernels1x1) ? kernels1x1->m_kernelDepth : 0,
			(partialMaps) ? partialMaps->m_size : 0,
			(residualMaps) ? residualMaps->m_size : 0,
			outputMaps->m_size,
            (prev1x1Maps) ? prev1x1Maps->m_size : 0,
			(kernels1x1) ? (kernels1x1->m_kernelDepth * CO_HIGH_WATERMARK_FACTOR) : (outputMaps->m_outputMapDepth * CO_HIGH_WATERMARK_FACTOR),
			(residualMaps) ? residualMaps->m_residualMapDepth * RM_LOW_WATERMARK_FACTOR : 0,
			(partialMaps) ? partialMaps->m_partialMapDepth * PM_LOW_WATERMARK_FACTOR : 0,
            (prev1x1Maps) ? prev1x1Maps->m_prev1x1MapDepth * PV_LOW_WATERMARK_FACTOR : 0,
			(residualMaps) ? residualMaps->m_residualMapDepth * RM_FETCH_FACTOR : 0,
			(partialMaps) ? partialMaps->m_partialMapDepth * PM_FETCH_FACTOR : 0,
            (prev1x1Maps) ? prev1x1Maps->m_prev1x1MapDepth * PV_FETCH_FACTOR : 0,
			krnl1x1_pding,
			krnl1x1_pad_bgn,
			krnl1x1_pad_end
		));
		m_accelCfg->m_FAS_cfg_arr[i]->m_partMapAddr = (partialMaps != nullptr) ? partialMaps->m_address : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_resMapAddr = (residualMaps != nullptr) ? residualMaps->m_address : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapAddr = outputMaps->m_address;
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
					bool cascade = (remDepth > QUAD_MAX_DEPTH) ? true : false;
					bool master_QUAD = (k == (MAX_QUAD_PER_AWP - 1) || !cascade) ? true : false;
					QUAD_cfg* quad_cfg = new QUAD_cfg(
						i,
						j,
						k,
						true,
						inputMaps->m_numInputMapRows,
						inputMaps->m_numInputMapCols,
						(kernels3x3) ? kernels3x3->m_numKernels : 0,
						(kernels3x3) ? kernels3x3->m_kernelDepth : 0,
						(kernels3x3) ? kernels3x3->m_numKernelRows : 0,
						(kernels3x3) ? kernels3x3->m_numKernelCols : 0,
						stride,
						upsample,
						padding,
						activation,
						master_QUAD,
						cascade,
						(remDepth > QUAD_MAX_DEPTH) ? QUAD_MAX_DEPTH : remDepth
					);
					QUAD_cfg_arr.push_back(quad_cfg);
					QUAD_en_arr.push_back(true);
					int imDepthStep = QUAD_MAX_DEPTH * inputMaps->m_numInputMapRows * inputMaps->m_numInputMapCols;
					int krn3x3DepthStep = QUAD_MAX_DEPTH * 3 * 3;
					imAddrArr[j][k] = inputMaps->m_address + (k * imDepthStep);
					krnl3x3AddrArr[j][k] = (kernels3x3) ? kernels3x3->m_address + (k * krn3x3DepthStep) : 0;
					krnl3x3BiasAddrArr[j][k] = (kernels3x3Bias) ? kernels3x3Bias->m_address : 0;
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
	delete m_pxSeqCfg;
	delete m_accelCfg;
	delete m_inputMaps;
	delete m_kernels3x3;
	if(m_residualMaps)
	{
		delete m_residualMaps;
	}
	delete m_outputMaps;
	delete m_kernels3x3Bias;
	if(m_kernels1x1)
	{
		delete m_kernels1x1;
	}
	if(m_kernels1x1Bias)
	{
		delete m_kernels1x1Bias;
	}
}
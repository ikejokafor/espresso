#include "Layer_Iteration.hpp"
using namespace std;


Layer_Iteration::Layer_Iteration(
	bool first_depth_iter,
	bool last_depth_iter, 
	InputMaps* inputMaps, 
	Kernels* kernels3x3,
	Kernels* kernels1x1,
	KernelBias* kernels3x3Bias,
	KernelBias* kernels1x1Bias,
	PartialMaps* partialMaps,
	ResidualMaps* residualMaps,
	OutputMaps* outputMaps, 
	int stride, 
	bool upsample, 
	int padding, 
	bool do_kernels1x1, 
	bool do_res_layer, 
	bool activation
) {
	m_pxSeqCfg = new PixelSeqCfg(stride);
	m_accelCfg = new AccelConfig();
	m_inputMaps	= inputMaps;
	m_kernels3x3 = kernels3x3;
	m_kernels1x1 = kernels1x1;
	m_kernels3x3Bias = kernels3x3Bias;
	m_kernels1x1Bias = kernels1x1Bias;
	m_partialMaps = partialMaps;
	m_residualMaps = residualMaps;
	m_outputMaps =	outputMaps;

	m_inputMaps->serialize();
	m_kernels3x3->serialize();
	m_kernels3x3Bias->serialize();
	m_outputMaps->serialize();
	(do_kernels1x1) ? m_kernels1x1->serialize() : void();
	(do_kernels1x1) ? m_kernels1x1Bias->serialize() : void();
	(!first_depth_iter) ? m_partialMaps->serialize() : void();
	(do_res_layer) ? m_residualMaps->serialize() : void();

	int remDepth = inputMaps->m_inputMapDepth;
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(
			i, 
			do_kernels1x1, 
			do_res_layer, 
			first_depth_iter, 
			last_depth_iter,
			m_pxSeqCfg->m_address,
			(do_kernels1x1) ? kernels1x1Bias->m_address : 0,
			(!first_depth_iter) ? partialMaps->m_address : 0,
			(do_res_layer) ? residualMaps->m_address : 0,
			outputMaps->m_address,
			m_pxSeqCfg->m_size,
			inputMaps->m_size,
			kernels3x3->m_size,
			(do_kernels1x1) ? kernels1x1->m_size : 0,		
			kernels3x3Bias->m_size,
			(do_kernels1x1) ? kernels1x1Bias->m_size : 0,
			(!first_depth_iter) ? partialMaps->m_size : 0,
			(do_res_layer) ? residualMaps->m_size : 0,
			outputMaps->m_size
		));
		m_accelCfg->m_FAS_cfg_arr[i]->m_partMapAddr = (partialMaps != nullptr) ? partialMaps->m_address : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_resMapAddr = (residualMaps != nullptr) ? residualMaps->m_address : -1;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapAddr = outputMaps->m_address;
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr.push_back(new AWP_cfg(i ,j));
			auto& imAddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_imAddrArr;
			auto& krnl3x3AddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_krnl3x3AddrArr;
			auto& krnl3x3BiasAddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_krnl3x3BiasAddrArr;
			for (int k = 0; k < MAX_QUAD_PER_AWP; k++)
			{
				auto& QUAD_cfg_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr;
				auto& QUAD_en_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_en_arr;
				if (remDepth > 0)
				{
					bool master_QUAD = (k == 0) ? true : false;
					bool cascade = true;
					QUAD_cfg* quad_cfg = new QUAD_cfg(
						i,
						j,
						k,
						true,
						inputMaps->m_numInputMapRows, 
						inputMaps->m_numInputMapCols,
						kernels3x3->m_numKernels,
						kernels3x3->m_kernelDepth,
						kernels3x3->m_numKernelRows,
						kernels3x3->m_numKernelCols,
						stride,
						upsample,
						padding,
						activation,
						master_QUAD,
						cascade
					);				
					QUAD_cfg_arr.push_back(quad_cfg);
					QUAD_en_arr.push_back(true);
					int imDepthStep = QUAD_MAX_DEPTH * inputMaps->m_numInputMapRows * inputMaps->m_numInputMapCols;
					int krn3x3DepthStep = QUAD_MAX_DEPTH * 3 * 3;
					imAddrArr[k] = inputMaps->m_address + (k * imDepthStep);
					krnl3x3AddrArr[k] = kernels3x3->m_address + (k * krn3x3DepthStep);
					krnl3x3BiasAddrArr[k] = kernels3x3Bias->m_address;
				}
				else
				{
					QUAD_cfg_arr.push_back(new QUAD_cfg(i, j, k, false));
					QUAD_en_arr.push_back(false);
				}
				remDepth -= QUAD_DPTH_SIMD;
			}
			m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_en_arr.push_back(true);
		}
	}
	m_accelCfg->serialize();
}


Layer_Iteration::~Layer_Iteration()
{

}
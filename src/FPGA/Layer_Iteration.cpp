#include "Layer_Iteration.hpp"
using namespace std;


Layer_Iteration::Layer_Iteration(
	int iter,
	bool first_depth_iter,
	bool last_depth_iter, 
	InputMaps* inputMaps, 
	Kernels* kernels3x3,
	Kernels* kernels1x1,
	Bias* bias3x3,
	Bias* bias1x1,
	PartialMaps* partialMaps,
	ResidualMaps* residualMaps,
	OutputMaps* outputMaps, 
	int stride, 
	bool upsample, 
	int padding, 
	bool do_kernel1x1, 
	bool do_res_layer, 
	bool activation,
	int inMapFetchTotal,
	int partMapFetchTotal,
	int krnl1x1FetchTotal,
	int krnl3x3FetchTotal,
	int resMapFetchTotal,
	int outMapStoreTotal
) {
	int remDepth = inputMaps->m_inputMapDepth;
	m_accelCfg = new AccelConfig();
	m_inputMaps	= inputMaps;
	m_kernels3x3 = kernels3x3;
	m_kernels1x1 = kernels1x1;
	m_partialMaps = partialMaps;
	m_residualMaps = residualMaps;
	m_outputMaps =	outputMaps;

	for(int i = 0; i < NUM_FAS; i++)
	{
		m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(
			i, 
			do_kernel1x1, 
			do_res_layer, 
			first_depth_iter, 
			last_depth_iter,
			inMapFetchTotal,
			partMapFetchTotal,
			krnl1x1FetchTotal,
			krnl3x3FetchTotal,
			resMapFetchTotal,
			outMapStoreTotal
		));
		m_accelCfg->m_FAS_cfg_arr[i]->m_partMapAddr = partialMaps->m_address;
		m_accelCfg->m_FAS_cfg_arr[i]->m_resMapAddr = residualMaps->m_address;
		m_accelCfg->m_FAS_cfg_arr[i]->m_outMapAddr = outputMaps->m_address;
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			auto imAddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_imAddrArr;
			auto krnl3x3AddrArr = m_accelCfg->m_FAS_cfg_arr[i]->m_krnl3x3AddrArr;
			m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr.push_back(new AWP_cfg(i ,j));
			for (int k = 0; k < NUM_QUAD_PER_AWP; k++)
			{
				auto QUAD_cfg_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_cfg_arr;
				auto QUAD_en_arr = m_accelCfg->m_FAS_cfg_arr[i]->m_AWP_cfg_arr[j]->m_QUAD_en_arr;
				if (remDepth > 0)
				{
					bool master_QUAD = (NUM_QUAD_PER_AWP  == 1) ? true
									   	: (NUM_QUAD_PER_AWP > 1 && k == 0) ? true : false;
					bool cascade = (NUM_QUAD_PER_AWP == 1) ? false
										: (NUM_QUAD_PER_AWP > 1) ? true : false;
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
					int krnDepthStep = QUAD_MAX_DEPTH * 3 * 3;
					imAddrArr[k] = inputMaps->m_address + (k * imDepthStep);
					krnl3x3AddrArr[k] = kernels3x3->m_address + (k * krnDepthStep);
				}
				else
				{
					QUAD_cfg_arr.push_back(new QUAD_cfg(false, i, j, k, false));
					QUAD_en_arr.push_back(false);
				}
				remDepth -= QUAD_DPTH_SIMD;
			}
		}
	}
}


Layer_Iteration::~Layer_Iteration()
{

}
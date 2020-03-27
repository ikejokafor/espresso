#include "Layer_Iteration.hpp"
using namespace std;


Layer_Iteration::Layer_Iteration(int iter, bool last_iter, InputMaps* inputMaps, Kernels* kernels, int stride, bool upsample, int padding, bool kernel_1x1, bool residual, bool activation)
{
	int remDepth = inputMaps->m_inputMapDepth;
	m_accelCfg = new AccelConfig();
	for(int i = 0; i < NUM_FAS; i++)
	{
		if (iter == 0)
		{
			m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(i, kernel_1x1, residual, true, false));
		}
		else if (iter > 0 && !last_iter)
		{
			m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(i, kernel_1x1, residual, false, false));
		}
		else if(last_iter)
		{
			m_accelCfg->m_FAS_cfg_arr.push_back(new FAS_cfg(i, kernel_1x1, residual, false, true));
		}
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
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
						kernels->m_numKernels,
						kernels->m_kernelDepth,
						kernels->m_numKernelRows,
						kernels->m_numKernelCols,
						stride,
						upsample,
						padding,
						activation,
						master_QUAD,
						cascade
					);				
					QUAD_cfg_arr.push_back(quad_cfg);
					QUAD_en_arr.push_back(true);
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
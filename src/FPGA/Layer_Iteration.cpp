#include "Layer_Iteration.hpp"


Layer_Iteration::Layer_Iteration(InputMaps* inputMaps, Kernels* kernels, bool upsample, int padding, int stride)
{
	m_fas_cfg_arr.resize(NUM_FAS);
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_fas_cfg_arr.push_back(new FAS_cfg(i));
		m_fas_cfg_arr[i]->m_awp_cfg_arr.resize(MAX_AWP_PER_FAS);
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			m_fas_cfg_arr[i]->m_awp_cfg_arr.push_back(new AWP_cfg(i ,j));
			m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_quad_cfg_arr.resize(NUM_QUAD_PER_AWP);
			for (int k = 0; k < NUM_QUAD_PER_AWP; k++)
			{
				QUAD_cfg* quad_cfg = new QUAD_cfg(
					i,
					j,
					k,
					inputMaps->m_numInputMapRows, 
					inputMaps->m_numInputMapCols,
					kernels->m_numKernels,
					kernels->m_kernelDpeth,
					kernels->m_numKernelRows,
					kernels->m_numKernelCols,
					stride,
					upsample,
					padding
				);				
				m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_quad_cfg_arr.push_back(quad_cfg);
			}
		}
	}
}


Layer_Iteration::~Layer_Iteration()
{
	delete m_inputMaps;
	delete m_kernels;
}
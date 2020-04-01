#pragma once


#include <vector>
#include <iostream>
#include "InputMaps.hpp"
#include "Kernels.hpp"
#include "Bias.hpp"
#include "PartialMaps.hpp"
#include "ResidualMaps.hpp"
#include "OutputMaps.hpp"
#include "espresso_FPGA_common.hpp"
#include "AccelConfig.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"


class Layer_Iteration
{
	public:
		Layer_Iteration(
			bool first_depth_iter,
			bool last_depth_iter, 
			InputMaps* inputMaps, 
			Kernels* kernels3x3,
			Kernels* kernels1x1,
			Bias*	 bias3x3,
			Bias*	 bias1x1,
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
		);		
		~Layer_Iteration();
	
		AccelConfig* m_accelCfg			;		
		InputMaps* m_inputMaps			;
		Kernels* m_kernels3x3			;
		Kernels* m_kernels1x1			;
		Bias* bias3x3					;
		Bias* bias1x1					;
		PartialMaps* m_partialMaps		;
		ResidualMaps* m_residualMaps	;
		OutputMaps* m_outputMaps		;
};
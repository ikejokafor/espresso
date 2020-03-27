#pragma once


#include <vector>
#include <iostream>
#include "InputMaps.hpp"
#include "OutputMaps.hpp"
#include "espresso_FPGA_common.hpp"
#include "AccelConfig.hpp"
#include "Kernels.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"


class Layer_Iteration
{
	public:
	Layer_Iteration(int iter, bool last_iter, InputMaps* inputMaps, Kernels* kernels, int stride, bool upsample, int padding, bool kernel_1x1, bool residual, bool activation);
		~Layer_Iteration();
	

		AccelConfig* m_accelCfg			;		
		InputMaps* m_inputMaps			;
		Kernels* m_kernels				;
		OutputMaps* m_outputMaps		;
};
#pragma once


#include <vector>
#include <iostream>
#include "InputMaps.hpp"
#include "common.hpp"
#include "Kernels.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"


class Layer_Iteration
{
	public:
		Layer_Iteration(InputMaps* inputMaps, Kernels* kernels, int stride, bool upsample, int padding, bool conv_out_fmt0, bool residual, bool activation);
		~Layer_Iteration();
	
		std::vector<FAS_cfg*> m_fas_cfg_arr;
		InputMaps* m_inputMaps			;
		Kernels* m_kernels				;
};
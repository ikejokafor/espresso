#pragma once


#include <vector>
#include <iostream>
#include "PixelSeqCfg.hpp"
#include "InputMaps.hpp"
#include "Kernels.hpp"
#include "KernelBias.hpp"
#include "ResidualMaps.hpp"
#include "OutputMaps.hpp"
#include "PartialMaps.hpp"
#include "Prev1x1Maps.hpp"
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
            bool first_krnl_iter,
			InputMaps* inputMaps,
			Kernels* kernels3x3,
			Kernels* kernels1x1,
			KernelBias*	 kernels3x3Bias,
			KernelBias*	 kernels1x1Bias,
			PartialMaps* partialMaps,
			ResidualMaps* residualMaps,
			OutputMaps* outputMaps,
            Prev1x1Maps* prev1x1Maps,
			int stride,
			bool upsample,
			int padding,
			bool do_kernels1x1,
			bool do_res_layer,
			bool activation,
			bool krnl1x1_pding,
			int krnl1x1_pad_bgn,
			int krnl1x1_pad_end,
			bool krnl_1x1_layer,
            bool do_1x1_res,
            bool do_res_1x1
		);
		~Layer_Iteration();

		AccelConfig* m_accelCfg			;
		PixelSeqCfg* m_pxSeqCfg			;
		InputMaps* m_inputMaps			;
		Kernels* m_kernels3x3			;
		Kernels* m_kernels1x1			;
		KernelBias* m_kernels3x3Bias	;
		KernelBias* m_kernels1x1Bias	;
		PartialMaps* m_partialMaps		;
		ResidualMaps* m_residualMaps	;
		OutputMaps* m_outputMaps		;
		Prev1x1Maps* m_prev1x1Maps		;
};
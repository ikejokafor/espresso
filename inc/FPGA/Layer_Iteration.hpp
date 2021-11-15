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
#include "espresso_common.hpp"
#include "AccelConfig.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"


class Layer_Iteration
{
	public:
		Layer_Iteration(
			FPGA_hndl* fpga_hndl,
			opcode_t opcode,
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
            espresso::activation_t act3x3,
            espresso::activation_t act1x1,
            bool it_act3x3,
            bool it_act1x1,
            bool it_bias3x3,
            bool it_bias1x1,
			bool krnl1x1_pding,
			int krnl1x1_pad_bgn,
			int krnl1x1_pad_end,    
            std::string layerName,
            int kernel_i,
            int depth_i,
            bool first,
            bool last
		);
		~Layer_Iteration();
        void prepFPGAData();

        std::string m_layerName         ;
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
		opcode_t m_opcode				;
        
        bool m_krnl1x1_pding;
        int m_krnl1x1_pad_bgn;
        int m_krnl1x1_pad_end;
        espresso::activation_t m_act1x1;
        espresso::activation_t m_act3x3;
        bool m_it_act1x1;
        bool m_it_act3x3;
        bool m_it_bias1x1;
        bool m_it_bias3x3;
        bool m_upsample;
        int m_kernel_i;
        int m_depth_i;
        bool m_first;
        bool m_last;
        int m_stride;
        bool m_padding;
};
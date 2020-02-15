#pragma once

#define NUM_FAS					1
#define MAX_AWP_PER_FAS         4
#define NUM_QUAD_PER_AWP        4
#define NUM_TOTAL_QUADS         (MAX_AWP_PER_FAS * NUM_QUAD_PER_AWE)
#define QUAD_MAX_KERNELS        64
#define QUAD_MAX_INMAP_DEPTH    8
#define QUAD_INMAP_DPTH_SIMD    (QUAD_MAX_INMAP_DEPTH * QUAD_MAX_INMAP_DEPTH)
#define QUAD_MAX_KRNL_DEPTH     8
#define QUAD_KRNL_DPTH_SIMD     (QUAD_MAX_KRNL_DEPTH * QUAD_MAX_KRNL_DEPTH)


#include "InputMaps.hpp"
#include "Kernels.hpp"
#include "FAS_cfg.hpp"
#include "QUAD_cfg.hpp"
#include "AWP_cfg.hpp"
#include <vector>


class Layer_Iteration
{
	public:
		Layer_Iteration(InputMaps* inputMaps, Kernels* kernels, bool upsample, int padding, int stride);
		~Layer_Iteration();
	
		std::vector<FAS_cfg*> m_fas_cfg_arr;
		InputMaps* m_inputMaps			;
		Kernels* m_kernels				;
};
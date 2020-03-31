#pragma once


#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include "espresso_FPGA_common.hpp"
#include "Network.hpp"
#include "MyNetProto.hpp"
#include "SYSC_FPGA_shim.hpp"
#include "Layer_Iteration.hpp"
#include "fixedPoint.hpp"
#include "InputMaps.hpp"
#include "PartialMaps.hpp"
#include "ResidualMaps.hpp"
#include "Bias.hpp"
#include "OutputMaps.hpp"
#include "Kernels.hpp"



class Layer_Job
{
    public:
        Layer_Job(
			std::string layerName,
		    int inputMapDepth, 
		    int numInputMapRows, 
		    int numInputMapCols,
			fixedPoint_t* inputMapData,
		    int numKernels, 
		    int kernelDepth, 
		    int numKernelRows, 
		    int numKernelCols,
			fixedPoint_t* kernelData,
			int outputMapDepth,
			int numOutputMapRows,
			int numOutputMapCols,
			fixedPoint_t* outputMapData,
			int residualMapDepth,
			int numResidualMapRows,
			int numResidualMapCols,
			fixedPoint_t* residualMapData,
			fixedPoint_t* kernel1x1Data,
			fixedPoint_t* bias3x3Data,
			fixedPoint_t* bias1x1Data,
			int stride,
			bool upsample,
			int padding,
			bool do_res_layer,
			bool activation,
			bool do_kernel1x1,
			FPGA_hndl* fpga_hndl,
		    int fxPtLength = 16,
		    int numFracBits = 14
	    );
        ~Layer_Job();
        void createLayerIters();
		void printConfig();
        void process();
        
		std::string m_layerName;
        int m_inputMapDepth;
        int m_numInputMapRows;
        int m_numInputMapCols;
		int m_outputMapDepth;
		int m_numOutputMapRows;	
		int m_numOutputMapCols;	
        int m_numKernels;
        int m_kernelDepth;
        int m_numKernelRows;
        int m_numKernelCols;
	    int m_fxPtLength;
        int m_numFracBits;
		int m_stride;
		bool m_upsample;
		bool m_padding;
		bool m_do_res_layer;
		bool m_do_kernel1x1;
		bool m_activation;
	    InputMaps* m_inputMaps;
	    Kernels* m_kernels3x3;
		Kernels* m_kernels1x1;
		ResidualMaps* m_residualMaps;
		OutputMaps*	m_outputMaps;
		Bias* m_bias3x3Data;
		Bias* m_bias1x1Data;
		std::vector<std::vector<Layer_Iteration*>> m_lay_it_arr;
		int m_num_Depth_iter;
		int m_num_krnl_iter;
#ifdef SYSTEMC
		SYSC_FPGA_hndl* m_sysc_fpga_hndl;
#else
	
#endif
};
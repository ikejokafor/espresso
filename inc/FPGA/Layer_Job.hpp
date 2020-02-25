#pragma once


#include <vector>
#include "fixedPoint.hpp"
#include "InputMaps.hpp"
#include "OutputMaps.hpp"
#include "Kernels.hpp"
#include <math.h> 
#include "Layer_Iteration.hpp"
#include "common.hpp"


class Layer_Job
{
    public:
        Layer_Job(
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
			int stride,
			bool upsample,
			int padding,
			bool conv_out_fmt0,
			bool residual,
			bool activation,
		    int fxPtLength = 16,
		    int numFracBits = 14
	    );
        ~Layer_Job();
        void createLayerIters();
        void process();
        
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
		bool m_conv_out_fmt0;
		bool m_residual;
		bool m_activation;
	    InputMaps* m_inputMaps;
	    Kernels* m_kernels;
		OutputMaps*	m_outputMaps;
        std::vector<Layer_Iteration*> m_lay_it_arr;
};
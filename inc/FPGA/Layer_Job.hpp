#pragma once


#include <vector>
#include "InputMaps.hpp"
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
		    int numKernels, 
		    int kernelDepth, 
		    int numKernelRows, 
		    int numKernelCols,
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
        std::vector<Layer_Iteration*> m_lay_it_arr;
};
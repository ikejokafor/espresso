#pragma once


#include <vector>
#include "InputMaps.hpp"
#include "Kernels.hpp"
#include "Layer_Iteration.hpp"
#include "AWP.hpp"
#include "QUAD.hpp"


class Layer_Job
{
    public:
        Layer_Job(
		    int inputmapDepth, 
		    int numInputMapRows, 
		    int numInputMapCols, 
		    int numKernels, 
		    int kernelDepth, 
		    int numKernelRows, 
		    int numKernelCols,
			bool m_upsample,
			int m_padding,
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
		int m_upsample;
		bool m_padding;
	    InputMaps* m_inputMaps;
	    Kernels* m_kernels;
        std::vector<Layer_Iteration> m_lay_it_arr;
};
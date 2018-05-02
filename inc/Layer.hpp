#ifndef __LAYER_HPP__
#define __LAYER_HPP__


#define index2D(ROW, COL, A, r, c)                                  A[r * COL + c]
#define index3D(DEPTH, ROW, COL, A, d, r, c)                        A[(d * ROW + r) * COL + c]
#define index4D(DEPTH1, DEPTH0, ROW, COL, A, d1, d0, r, c)          A[((d1 * DEPTH0 + d0) * ROW + r) * COL + c]
#define ESPRO_DEF_FXPT_LEN                                          32
#define ESPRO_DEF_NUM_FRAC_BITS                                     16


#include <string>
#include <cstring>
#include <vector>
#include <random>
#include <float.h>
#include <iostream>
#include <fstream>
#include <stack>
#include "FixedPoint.hpp"


struct Blob_t {
    float *flData;
    FixedPoint_t *fxData;
    int blobSize;
	int numRows;
    int numCols;
	int depth;
};


typedef enum {
    FLOAT = 0,
    FIXED = 1
} precision_t;


namespace espresso {
    //	LayerTypes:
    //		Input
    //		Convolution
    //		ReLU
    //      LRN
    //		Pooling_MAX
    //		Pooling_AVE
    //		InnerProduct
    //		Softmax
    //		Concat
    
    
    struct layerInfo_t {
        precision_t precision;       
        std::string layerName;
        std::vector<std::string> topLayerNames;
        std::vector<std::string> bottomLayerNames;
        std::string layerType;
        int numInputRows;
        int numInputCols;
        int inputDepth;
        int outputDepth;
        int dinFxPtLength;
        int dinNumFracBits;
        int whtFxPtLength;
        int whtNumFracBits;
        int doutFxPtLength;
        int doutNumFracBits;       
        int numKernelRows;
        int numKernelCols;
        int stride;
        int padding;
        bool globalPooling;
        float *flFilterData;
        float *flBiasData;
        int numFilterValues;
        FixedPoint_t *fxFilterData;
        FixedPoint_t *fxBiasData;
        int group;
        int localSize;
        float alpha;
        float beta;            
    };
}


// We can compute the spatial size of the output volume as a function of the input volume size (W), the
// the receptive field size of the Conv Layer neurons (F), the stride with which they are applied (S), and
// the amount of zero padding used (P) on the border.You can convince yourself that the correct formula for 
// calculating how many neurons “fit” is given by ((W - F + (2 * P)) / S) + 1. For example for a 7x7 input and a 
// 3x3 filter with stride 1 and pad 0 we would get a 5x5 output.With stride 2 we would get a 3x3 output.

// Blob memory is row-major in layout, so the last / rightmost dimension changes fastest. For example, in a 4D blob, the value at index (n, k, h, w) is physically 
// located at index ((n * K + k) * H + h) * W + w. Number / N is the batch size of the data. Batch processing achieves better throughput for communication and device processing. 
// For an ImageNet training batch of 256 images N = 256. Channel / K is the feature dimension e.g. for RGB images K = 3.



class Layer {

    public:
           Layer    (
                        precision_t precision = FLOAT, 
                        std::string layerName = " ",
                        std::vector<std::string> topLayerNames = std::vector<std::string>(),
                        std::vector<std::string> bottomLayerNames = std::vector<std::string>(),
                        std::string layerType = " ",
                        int numInputRows = 1,
                        int numInputCols = 1,
                        int inputDepth = 1,
                        int outputDepth = 1,
                        int dinFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int dinNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,
                        int whtFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int whtNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,
                        int doutFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int doutNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,                               
                        int numKernelRows = 1,
                        int numKernelCols = 1,
                        int stride = 1,
                        int padding = 0,
                        bool globalPooling = false,
                        float *flFilterData = NULL,
                        float *flBiasData = NULL,
                        FixedPoint_t *fxFilterData = NULL,
                        FixedPoint_t *fxBiasData = NULL,
                        int numFilterValues = 1,
                        int group = 1,
                        int localSize = 5,
                        float alpha = 0.0001f,
                        float beta = 0.75f                      
                    );
            virtual ~Layer();
            virtual void ComputeLayer() = 0;
            virtual void ComputeLayerParam() = 0;


        precision_t m_precision;
		std::string m_layerName;
		std::vector<std::string> m_topLayerNames;
		std::vector<std::string> m_bottomLayerNames;
		std::string m_layerType;
		int m_numInputRows;
		int m_numInputCols;
		int m_inputDepth;
		int m_numOutputRows;
		int m_numOutputCols;
		int m_outputDepth;
        int m_dinFxPtLength;
        int m_dinNumFracBits;
        int m_whtFxPtLength;
        int m_whtNumFracBits;  
        int m_doutFxPtLength;
        int m_doutNumFracBits;         
		int m_numKernelRows;
		int m_numKernelCols;
		int m_kernelDepth;
		int m_numKernels;
		int m_stride;
		int m_padding;
        bool m_globalPooling;
		float *m_flFilterData;
		float *m_flBiasData;
		FixedPoint_t *m_fxFilterData;
		FixedPoint_t *m_fxBiasData;
        int m_numFilterValues;
        int m_group;      
        int m_localSize;
        float m_alpha;
        float m_beta;

        
		Blob_t m_blob;
		std::vector<Layer*> m_topLayers;
		std::vector<Layer*> m_bottomLayers;


    protected:


    private:

};

#endif
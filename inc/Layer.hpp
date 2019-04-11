#ifndef __LAYER_HPP__
#define __LAYER_HPP__


#define ESPRO_DEF_FXPT_LEN          32
#define ESPRO_DEF_NUM_FRAC_BITS		16
#define HIGH_PERF


#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <random>
#include <float.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>

#include "util.hpp"
#include "fixedPoint.hpp"
#include <functional>


struct Blob_t {
    float *flData;
    fixedPoint_t *fxData;
    int blobSize;
	int numRows;
    int numCols;
	int depth;
};


namespace espresso {
	typedef enum {
		FLOAT = 0,
		FIXED = 1
	} precision_t;


	typedef enum {
		LEAKY = 0,
		RELU  = 1,
		LINEAR = 2
	} activation_t;
	
    //	LayerTypes:
    //		Input
    //		Convolution
    //		Activation
    //      LRN
    //		Pooling_MAX
    //		Pooling_AVG
    //		InnerProduct
    //		Softmax
    //		Concat
	//		Residual
	//		YOLO
	//		UpSample
	
    
    struct layerInfo_t {
		layerInfo_t() :
		    precision(espresso::FLOAT), 
			layerName(" "),
			topLayerNames(std::vector<std::string>()),
			bottomLayerNames(std::vector<std::string>()),
			layerType(" "),
			numInputRows(1),
			numInputCols(1),
			inputDepth(1),
			outputDepth(1),
			dinFxPtLength(ESPRO_DEF_FXPT_LEN),
			dinNumFracBits(ESPRO_DEF_NUM_FRAC_BITS),
			whtFxPtLength(ESPRO_DEF_FXPT_LEN),
			whtNumFracBits(ESPRO_DEF_NUM_FRAC_BITS),
			doutFxPtLength(ESPRO_DEF_FXPT_LEN),
			doutNumFracBits(ESPRO_DEF_NUM_FRAC_BITS),	
			biasFxPtLength(ESPRO_DEF_FXPT_LEN),
			biasNumFracBits(ESPRO_DEF_NUM_FRAC_BITS),
		    leakyFxPtLength(ESPRO_DEF_FXPT_LEN),
		    leakyNumFracBits(ESPRO_DEF_NUM_FRAC_BITS),
			numKernelRows(1),
			numKernelCols(1),
			stride(1),
			padding(0),
			globalPooling(false),
		    numFilterValues(1),
			flFilterData(NULL),
			flBiasData(NULL),
			fxFilterData(NULL),
			fxBiasData(NULL),
			group(1),
			localSize(5),
			alpha(0.0001f),
			flBeta(0.75f),
		    fxBeta(0.000001f),
			activation(espresso::RELU),
		    darknetNormScaleBias(false),
		    darknetAct(false),
		    darknet_n_param(0),
			darknet_classes_param(0),
		    darknet_outputs_param(0)
		    {}
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
	    int biasFxPtLength;
        int biasNumFracBits;
	    int leakyFxPtLength;
	    int	leakyNumFracBits;
        int numKernelRows;
        int numKernelCols;
        int stride;
        int padding;
        bool globalPooling;
        int numFilterValues;	    
        float *flFilterData;
        float *flBiasData;
        fixedPoint_t *fxFilterData;
        fixedPoint_t *fxBiasData; 
        int group;
        int localSize;
        float alpha;
        float flBeta;
	    float fxBeta;
		espresso::activation_t activation;
	    bool darknetNormScaleBias;
	    bool darknetAct;
	    int darknet_n_param;
	    int darknet_classes_param;
	    int darknet_outputs_param;
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
        Layer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        virtual ~Layer();
        virtual void ComputeLayerParam() = 0;
        virtual void ComputeLayer() = 0;
	    virtual void ComputeLayer_FlPt() = 0;
        virtual void ComputeLayer_FxPt() = 0;


		espresso::precision_t m_precision;
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
		int m_biasFxPtLength;
		int m_biasNumFracBits;
	    int m_leakyFxPtLength;
	    int	m_leakyNumFracBits;
		int m_numKernelRows;
		int m_numKernelCols;
		int m_kernelDepth;
		int m_numKernels;
		int m_stride;
		int m_padding;
        bool m_globalPooling;
	    int m_numFilterValues;
		float *m_flFilterData;
		float *m_flBiasData;
		fixedPoint_t *m_fxFilterData;
		fixedPoint_t *m_fxBiasData;
        int m_group;      
        int m_localSize;
        float m_alpha;
        float m_flBeta;
	    float m_fxBeta;
		espresso::activation_t m_activation;
		bool m_darknetAct;
		int m_darknet_n_param;
	    int m_darknet_classes_param;
		int m_darknet_outputs_param;
		bool m_darknetNormScaleBias;
        
		Blob_t m_blob;
		std::vector<Layer*> m_topLayers;
		std::vector<Layer*> m_bottomLayers;


    protected:


    private:

};

#endif
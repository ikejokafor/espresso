#include "SoftMaxLayer.hpp"
using namespace std;


SoftMaxLayer::SoftMaxLayer  (
                                precision_t precision,
                                string layerName,
                                vector<string> topLayerNames,
                                vector<string> bottomLayerNames,
                                string layerType,
                                int numInputRows,
                                int numInputCols,
                                int inputDepth,
                                int outputDepth,
                                int numKernelRows,
                                int numKernelCols,
                                int stride,
                                int padding,
                                bool globalPooling,
                                float *flFilterData,
                                float *flBiasData,  
                                FixedPoint_t *fxFilterData,
                                FixedPoint_t *fxBiasData,                                    
                                int group,
                                int localSize,
                                float alpha,
                                float beta,
                                int dinFxPtLength,
                                int dinNumFracBits,
                                int whtFxPtLength,
                                int whtNumFracBits
                            ) : Layer	(	
                                                    precision,
                                                    layerName,
                                                    topLayerNames,
                                                    bottomLayerNames,
                                                    layerType,
                                                    numInputRows,
                                                    numInputCols,
                                                    inputDepth,
                                                    outputDepth,
                                                    numKernelRows,
                                                    numKernelCols,
                                                    stride,
                                                    padding,
                                                    globalPooling,
                                                    flFilterData,
                                                    flBiasData,                                                          
                                                    fxFilterData,                                                        
                                                    fxBiasData,                                                          
                                                    group,
                                                    localSize,
                                                    alpha,
                                                    beta,
                                                    dinFxPtLength,
                                                    dinNumFracBits,
                                                    whtFxPtLength,
                                                    whtNumFracBits
                                                ) {
}


SoftMaxLayer::~SoftMaxLayer() {
    free(m_blob.flData);
}


void SoftMaxLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth   = m_inputDepth;
	m_numOutputCols = m_numInputCols;
	m_numOutputRows = m_numInputRows;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
}


void SoftMaxLayer::ComputeLayer() { // made with dcNet in mind, may need to change for general Neural networks with common softmax layers

	// get input
	float *datain = m_bottomLayers[0]->m_blob.flData;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.flData;
    
    // adpated from caffe softmax_layer.cpp; Code which sets outer_num_ and inner_num_ will need to change if a softmax layer specifies a softmax axis which is not 1
    int channels = m_inputDepth;
    int outer_num_ = 1;
    int inner_num_ = numInputBlobRows * numInputBlobCols;
    int dim = (inputBlobDepth * numInputBlobRows * numInputBlobCols) / outer_num_;
    float *maxs = (float*)malloc(inner_num_ * sizeof(float));
    float *sums = (float*)malloc(inner_num_ * sizeof(float));

    for (int i = 0; i < outer_num_; ++i) {
        memcpy(maxs, (datain + i * dim), sizeof(float) * inner_num_);
        for (int j = 0; j < channels; j++) {
            for (int k = 0; k < inner_num_; k++) {
                maxs[k] = std::max(maxs[k],
                    datain[i * dim + j * inner_num_ + k]);
            }
        }
    }
    
    memcpy(dataout, datain, dim * sizeof(float));
    
    // subtraction
    float *ptr = (float*)dataout;
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            ptr[j] -= maxs[j];
        }
        ptr += inner_num_;
    }
    
    // exponentiation
    ptr = (float*)dataout;
    for(int i = 0; i < (channels * inner_num_); i++) {
        ptr[i] = exp(ptr[i]);
    }    
    
    // sum after exp
    ptr = (float*)dataout;
    memset(sums, 0,  numInputBlobRows * numInputBlobCols * sizeof(float));
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            sums[j] += ptr[j];
        }
        ptr += inner_num_;
    }
    
    
    // division
    ptr = (float*)dataout;
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            ptr[j] /= sums[j];
        }
        ptr += inner_num_;
    }

    free(maxs);
    free(sums);

}

#include "SoftMaxLayer.hpp"
using namespace std;


template <typename DType>
SoftMaxLayer<DType>::SoftMaxLayer	(
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
                                    DType *filterData,
                                    DType *biasData,
                                    int group,
                                    int localSize,
                                    float alpha,
                                    float beta,
                                    int fxPtLength,
                                    int numFracBits
                                ) : Layer<DType>	(	
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
                                                        filterData,
                                                        biasData,
                                                        group,
                                                        localSize,
                                                        alpha,
                                                        beta,
                                                        fxPtLength,
                                                        numFracBits
                                                    ) {
}

template <typename DType>
SoftMaxLayer<DType>::~SoftMaxLayer() {
    free(this->m_blob.data);
}


template <typename DType>
void SoftMaxLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

	// output size
	this->m_outputDepth   = this->m_inputDepth;
	this->m_numOutputCols = this->m_numInputCols;
	this->m_numOutputRows = this->m_numInputRows;

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <typename DType>
void SoftMaxLayer<DType>::ComputeLayer() { // made with dcNet in mind, may need to change for general Neural networks with common softmax layers

	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;

	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
    
    // adpated from caffe softmax_layer.cpp; Code which sets outer_num_ and inner_num_ will need to change if a softmax layer specifies a softmax axis which is not 1
    int channels = this->m_inputDepth;
    int outer_num_ = 1;
    int inner_num_ = numInputBlobRows * numInputBlobCols;
    int dim = (inputBlobDepth * numInputBlobRows * numInputBlobCols) / outer_num_;
    DType *maxs = (DType*)malloc(inner_num_ * sizeof(DType));
    DType *sums = (DType*)malloc(inner_num_ * sizeof(DType));

    for (int i = 0; i < outer_num_; ++i) {
        memcpy(maxs, (datain + i * dim), sizeof(DType) * inner_num_);
        for (int j = 0; j < channels; j++) {
            for (int k = 0; k < inner_num_; k++) {
                maxs[k] = std::max(maxs[k],
                    datain[i * dim + j * inner_num_ + k]);
            }
        }
    }
    
    memcpy(dataout, datain, dim * sizeof(DType));
    
    // subtraction
    DType *ptr = (DType*)dataout;
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            ptr[j] -= maxs[j];
        }
        ptr += inner_num_;
    }
    
    // exponentiation
    ptr = (DType*)dataout;
    for(int i = 0; i < (channels * inner_num_); i++) {
        ptr[i] = exp(ptr[i]);
    }    
    
    // sum after exp
    ptr = (DType*)dataout;
    memset(sums, 0,  numInputBlobRows * numInputBlobCols * sizeof(DType));
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            sums[j] += ptr[j];
        }
        ptr += inner_num_;
    }
    
    
    // division
    ptr = (DType*)dataout;
    for(int i = 0; i < channels; i++) {
        for(int j = 0; j < inner_num_; j++) {
            ptr[j] /= sums[j];
        }
        ptr += inner_num_;
    }

    free(maxs);
    free(sums);

}


template <>
void SoftMaxLayer<FixedPoint>::ComputeLayer() { // made with dcNet in mind, may need to change for general Neural networks with common softmax layers

    // TODO:
	// get input
	//DType *datain = this->m_bottomLayers[0]->m_blob.data;
	//int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	//int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	//int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;
    //
	//// output
	//DType *dataout = this->m_topLayers[0]->m_blob.data;
    //
    //// adpated from caffe softmax_layer.cpp; Code which sets outer_num_ and inner_num_ will need to change if a softmax layer specifies a softmax axis which is not 1
    //int channels = this->m_inputDepth;
    //int outer_num_ = 1;
    //int inner_num_ = numInputBlobRows * numInputBlobCols;
    //int dim = (inputBlobDepth * numInputBlobRows * numInputBlobCols) / outer_num_;
    //DType *maxs = (DType*)malloc(inner_num_ * sizeof(DType));
    //DType *sums = (DType*)malloc(inner_num_ * sizeof(DType));
    //
    //for (int i = 0; i < outer_num_; ++i) {
    //    memcpy(maxs, (datain + i * dim), sizeof(DType) * inner_num_);
    //    for (int j = 0; j < channels; j++) {
    //        for (int k = 0; k < inner_num_; k++) {
    //            maxs[k] = std::max(maxs[k],
    //                datain[i * dim + j * inner_num_ + k]);
    //        }
    //    }
    //}
    //
    //memcpy(dataout, datain, dim * sizeof(DType));
    //
    //// subtraction
    //DType *ptr = (DType*)dataout;
    //for(int i = 0; i < channels; i++) {
    //    for(int j = 0; j < inner_num_; j++) {
    //        ptr[j] -= maxs[j];
    //    }
    //    ptr += inner_num_;
    //}
    //
    //// exponentiation
    //ptr = (DType*)dataout;
    //for(int i = 0; i < (channels * inner_num_); i++) {
    //    ptr[i] = exp(ptr[i]);
    //}    
    //
    //// sum after exp
    //ptr = (DType*)dataout;
    //memset(sums, 0,  numInputBlobRows * numInputBlobCols * sizeof(DType));
    //for(int i = 0; i < channels; i++) {
    //    for(int j = 0; j < inner_num_; j++) {
    //        sums[j] += ptr[j];
    //    }
    //    ptr += inner_num_;
    //}
    //
    //
    //// division
    //ptr = (DType*)dataout;
    //for(int i = 0; i < channels; i++) {
    //    for(int j = 0; j < inner_num_; j++) {
    //        ptr[j] /= sums[j];
    //    }
    //    ptr += inner_num_;
    //}
    //
    //free(maxs);
    //free(sums);

}


template class SoftMaxLayer<float>;
template class SoftMaxLayer<FixedPoint>;

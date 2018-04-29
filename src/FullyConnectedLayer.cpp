#include "FullyConnectedLayer.hpp"
using namespace std;


template <typename DType>
FullyConnectedLayer<DType>::FullyConnectedLayer	(
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
                                                    DType *filterData,
                                                    DType *biasData,
                                                    int group,
                                                    int localSize,
                                                    float alpha,
                                                    float beta,
                                                    int fxPtLength,
                                                    int numFracBits
                                                ) : Layer<DType>	(	
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
FullyConnectedLayer<DType>::~FullyConnectedLayer() {
    free(this->m_blob.data);
}


template <typename DType>
void FullyConnectedLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

	// output size
	this->m_numOutputRows = 1;
	this->m_numOutputCols = 1;
    
    // kernel depth
    this->m_kernelDepth = this->m_inputDepth * this->m_numInputRows * this->m_numInputCols;
    this->m_numKernels = this->m_outputDepth;
    
	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <typename DType>
void FullyConnectedLayer<DType>::ComputeLayer() {

	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
    
	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
    
	for (int m = 0; m < this->m_numKernels; m++) {
        dataout[m] = this->m_biasData[m];
        for (int k = 0; k < this->m_kernelDepth; k++) {
            dataout[m] += datain[k] * index2D(this->m_numKernels, this->m_kernelDepth, this->m_filterData, m, k);
        }
	}

}


template class FullyConnectedLayer<float>;
template class FullyConnectedLayer<FixedPoint_t>;


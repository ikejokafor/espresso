#include "ConcatLayer.hpp"
using namespace std;


template <typename DType>
ConcatLayer<DType>::ConcatLayer	(
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
                                    DType *filterData,
                                    DType *biasData,
                                    int length,
                                    int numFracbits
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
                                                        filterData,
                                                        biasData,
                                                        length,
                                                        numFracbits
                                                    ) {
}


template <typename DType>
ConcatLayer<DType>::~ConcatLayer() {
    free(this->m_blob.data);
}


template <typename DType>
void ConcatLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

	// output size;
	this->m_numOutputRows = this->m_numInputRows;
	this->m_numOutputCols = this->m_numInputCols;
	this->m_outputDepth	= this->m_bottomLayers[0]->m_outputDepth;
	for (uint32_t i = 1; i < this->m_bottomLayers.size(); i++) {
		this->m_outputDepth += this->m_bottomLayers[i]->m_outputDepth;
	}

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <typename DType>
void ConcatLayer<DType>::ComputeLayer() {

	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;

	for (uint32_t i = 0; i < this->m_bottomLayers.size(); i++) {
		memcpy(dataout, this->m_bottomLayers[i]->m_blob.data, this->m_bottomLayers[i]->m_blob.depth * this->m_bottomLayers[i]->m_blob.numRows * this->m_bottomLayers[i]->m_blob.numCols * sizeof(DType));
		dataout += (this->m_bottomLayers[i]->m_blob.depth * this->m_bottomLayers[i]->m_blob.numRows * this->m_bottomLayers[i]->m_blob.numCols);
	}

}


template class ConcatLayer<float>;
template class ConcatLayer<FixedPoint>;

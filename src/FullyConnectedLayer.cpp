#include "FullyConnectedLayer.hpp"
using namespace std;


template <typename DType>
FullyConnectedLayer<DType>::FullyConnectedLayer	(
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
	this->m_numOutputRows = this->m_inputDepth * this->m_numInputRows * this->m_numInputCols;
	this->m_numOutputCols = 1;

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <typename DType>
void FullyConnectedLayer<DType>::ComputeLayer() {

     TODO:
	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
    
	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
    
	for (int m = 0; m < this->m_numKernels; m++) {
		for (int x = 0; x < this->m_numOutputRows; x++) {
			index2D(this->m_outputDepth, this->m_numOutputRows, dataout, m, x) = this->m_biasData[m];
			for (int k = 0; k < this->m_inputDepth; k++) {
				index2D(this->m_outputDepth, this->m_numOutputRows, dataout, m, x) += datain[k] * index2D(this->m_numKernels, this->m_kernelDepth, this->m_filterData, m, k);
			}
		}
	}

}

template <>
void FullyConnectedLayer<FixedPoint>::ComputeLayer() {

    // TODO:
	//// get input
	//DType *datain = this->m_bottomLayers[0]->m_blob.data;
    //
	//// output
	//DType *dataout = this->m_topLayers[0]->m_blob.data;
    //
	//for (int m = 0; m < this->m_numKernels; m++) {
	//	for (int x = 0; x < this->m_numOutputRows; x++) {
	//		index2D(this->m_outputDepth, this->m_numOutputRows, dataout, m, x) = this->m_biasData[m];
	//		for (int k = 0; k < this->m_inputDepth; k++) {
	//			index2D(this->m_outputDepth, this->m_numOutputRows, dataout, m, x) += datain[k] * index2D(this->m_numKernels, this->m_kernelDepth, this->m_filterData, m, k);
	//		}
	//	}
	//}

}


template class FullyConnectedLayer<float>;
template class FullyConnectedLayer<FixedPoint>;


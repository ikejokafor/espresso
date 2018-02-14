#include "RELULayer.hpp"
using namespace std;


template <typename DType>
RELULayer<DType>::RELULayer	(
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
RELULayer<DType>::~RELULayer() {}


template <typename DType>
void RELULayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth   = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

	// output size
	this->m_outputDepth = this->m_inputDepth;
	this->m_numOutputRows = this->m_numInputRows;
	this->m_numOutputCols = this->m_numInputCols;
}


template <typename DType>
void RELULayer<DType>::ComputeLayer() {

	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;

	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
    FixedPoint zero = 0.0f;
   
	for (int m = 0; m < this->m_outputDepth; m++) {
		for (int x = 0; x < this->m_numOutputRows; x++) {
			for(int y = 0; y < this->m_numOutputCols; y++) {
				index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
					index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < zero 
				) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
			}
		}
	}

}


template class RELULayer<float>;
template class RELULayer<FixedPoint>;

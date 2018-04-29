#include "RELULayer.hpp"
using namespace std;


template <typename DType>
RELULayer<DType>::RELULayer (
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
    
	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)this->m_topLayers[0]->m_blob.data;    
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
   
	for (int m = 0; m < this->m_outputDepth; m++) {
		for (int x = 0; x < this->m_numOutputRows; x++) {
			for(int y = 0; y < this->m_numOutputCols; y++) {
				index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
					index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0 
				) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
			}
		}
	}

}


template class RELULayer<float>;
template class RELULayer<FixedPoint_t>;

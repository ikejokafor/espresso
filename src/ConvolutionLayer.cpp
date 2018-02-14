#include "ConvolutionLayer.hpp"
using namespace std;


template <typename DType>
ConvolutionLayer<DType>::ConvolutionLayer	(
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
ConvolutionLayer<DType>::~ConvolutionLayer() {
    free(this->m_blob.data);
}


template <typename DType>
void ConvolutionLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth	= this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows  = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols  = this->m_bottomLayers[0]->m_numOutputCols;

	// output size
	this->m_numOutputRows = (int)((this->m_numInputRows - this->m_numKernelRows + 2 * this->m_padding) / this->m_stride) + 1;
	this->m_numOutputCols = (int)((this->m_numInputCols - this->m_numKernelCols + 2 * this->m_padding) / this->m_stride) + 1;
	this->m_numKernels	= this->m_outputDepth;
	this->m_kernelDepth   = this->m_inputDepth;

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
    
}

template <typename DType>
void ConvolutionLayer<DType>::ComputeLayer() {

	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;
    
	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
        
	for (int m = 0; m < this->m_numKernels; m++) {
		for (int x = 0, a = 0; x < this->m_numOutputRows; x++, a += this->m_stride) {
			for(int y = 0, b = 0; y < this->m_numOutputCols; y++, b += this->m_stride) {
				index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = this->m_biasData[m];
				for (int k = 0; k < this->m_kernelDepth; k++) {
					for (int i = a - this->m_padding, kr = 0; kr < this->m_numKernelRows; i++, kr++) {
						for (int j = b - this->m_padding, kc = 0; kc < this->m_numKernelCols; j++, kc++) {
							if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding                        
                                index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) += (
                                    index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, i, j)
                                    * index4D(this->m_numKernels, this->m_kernelDepth, this->m_numKernelRows, this->m_numKernelCols, this->m_filterData, m, k, kr, kc)
                                );

                            }
						}
					}
				}
			}
		}
	}
}


template <>
void ConvolutionLayer<FixedPoint>::ComputeLayer() {

	// get input
	FixedPoint *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;
    
	// output
	FixedPoint *dataout = this->m_topLayers[0]->m_blob.data;
        
	for (int m = 0; m < this->m_numKernels; m++) {
		for (int x = 0, a = 0; x < this->m_numOutputRows; x++, a += this->m_stride) {
			for(int y = 0, b = 0; y < this->m_numOutputCols; y++, b += this->m_stride) {
				index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = this->m_biasData[m];
				for (int k = 0; k < this->m_kernelDepth; k++) {
					for (int i = a - this->m_padding, kr = 0; kr < this->m_numKernelRows; i++, kr++) {
						for (int j = b - this->m_padding, kc = 0; kc < this->m_numKernelCols; j++, kc++) {
							if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                FixedPoint product = FixedPoint::mult   (  
                                                                            index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, i, j),
                                                                            index4D(this->m_numKernels, this->m_kernelDepth, this->m_numKernelRows, this->m_numKernelCols, this->m_filterData, m, k, kr, kc),
                                                                            this->m_length,
                                                                            this->m_numFracbits
                                                                        );                               
                                index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) += (product);

                            }
						}
					}
				}
			}
		}
	}
}


template class ConvolutionLayer<float>;
template class ConvolutionLayer<FixedPoint>;

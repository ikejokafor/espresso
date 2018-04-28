#include "PoolingLayer.hpp"
using namespace std;


template <typename DType>
PoolingLayer<DType>::PoolingLayer   (
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
PoolingLayer<DType>::~PoolingLayer() {
    free(this->m_blob.data);
}


template <>
PoolingLayer<FixedPoint>::~PoolingLayer() {

}


template <typename DType>
void PoolingLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

    // output size
    this->m_outputDepth = this->m_inputDepth;
    if(this->m_globalPooling) {
        this->m_numOutputRows = 1;
        this->m_numOutputCols = 1;
    } else {
        this->m_numOutputRows = int(ceil(float(this->m_numInputRows - this->m_numKernelRows + 2.0f * this->m_padding) / float(this->m_stride))) + 1;
        this->m_numOutputCols = int(ceil(float(this->m_numInputCols - this->m_numKernelCols + 2.0f * this->m_padding) / float(this->m_stride))) + 1;
    }

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <>
void PoolingLayer<FixedPoint>::ComputeLayerParam() {
	// input size
	this->m_inputDepth = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

    // output size
    this->m_outputDepth = this->m_inputDepth;
    if(this->m_globalPooling) {
        this->m_numOutputRows = 1;
        this->m_numOutputCols = 1;
    } else {
        this->m_numOutputRows = int(ceil(float(this->m_numInputRows - this->m_numKernelRows + 2.0f * this->m_padding) / float(this->m_stride))) + 1;
        this->m_numOutputCols = int(ceil(float(this->m_numInputCols - this->m_numKernelCols + 2.0f * this->m_padding) / float(this->m_stride))) + 1;
    }

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (FixedPoint*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(FixedPoint));
    for(int i = 0; i < (this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols); i++) {
        this->m_blob.data[i].SetParam(this->m_fxPtLength, this->m_numFracBits);
    }
}


template <typename DType>
void PoolingLayer<DType>::ComputeLayer() {

	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;

	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
    
    if(this->m_globalPooling) {
        if(this->m_layerType == "Pooling_MAX") {
            // TODO
        } else if(this->m_layerType == "Pooling_AVE") { 
            // TODO
        }
    } else {
        if(this->m_layerType == "Pooling_MAX") {
            for (int m = 0; m < this->m_outputDepth; m++) {
                for (int x = 0, a = 0; x < this->m_numOutputRows; x++, a += this->m_stride) {
                    for (int y = 0, b = 0; y < this->m_numOutputCols; y++, b += this->m_stride) {
                        index3D(m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = -FLT_MAX;
                        for (int i = a - this->m_padding, kr = 0; kr < this->m_numKernelRows; i++, kr++) {
                            for (int j = b - this->m_padding, kc = 0; kc < this->m_numKernelCols; j++, kc++) {
                                if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                    index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
                                        index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) 
                                            < index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                        ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                        : index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y);
                                } else {
                                    index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
                                        index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) 
                                            < -FLT_MAX
                                        ) ? -FLT_MAX
                                        : index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y);
                                }
                            }
                        }
                    }
                }
            }
        } else if(this->m_layerType == "Pooling_AVE") { 
            for (int m = 0; m < this->m_outputDepth; m++) {
                for (int x = 0, a = 0; x < this->m_numOutputRows; x++, a += this->m_stride) {
                    for (int y = 0, b = 0; y < this->m_numOutputCols; y++, b += this->m_stride) {
                        DType ave = 0.0f;
                        for (int i = a - this->m_padding, kr = 0; kr < this->m_numKernelRows; i++, kr++) {
                            for (int j = b - this->m_padding, kc = 0; kc < this->m_numKernelCols; j++, kc++) {
                                if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                    ave += index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j);
                                }
                            }
                        }
                        index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = ave / DType(this->m_numKernelRows * this->m_numKernelCols);
                    }
                }
            }
        }
    }
}

template <>
void PoolingLayer<FixedPoint>::ComputeLayer() {
	// get input
	FixedPoint *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;

	// output
	FixedPoint *dataout = this->m_topLayers[0]->m_blob.data;
    
    FixedPoint fxPtMin(32, 16, 0x8000000000000000, false);
    
    // TODO: add support for average pooling and global pooling
    
    for (int m = 0; m < this->m_outputDepth; m++) {
        for (int x = 0, a = 0; x < this->m_numOutputRows; x++, a += this->m_stride) {
            for (int y = 0, b = 0; y < this->m_numOutputCols; y++, b += this->m_stride) {
                index3D(m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = fxPtMin;
                for (int i = a - this->m_padding, kr = 0; kr < this->m_numKernelRows; i++, kr++) {
                    for (int j = b - this->m_padding, kc = 0; kc < this->m_numKernelCols; j++, kc++) {
                        if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                            index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
                                index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) 
                                    < index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                : index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y);
                        } else {
                            index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) = (
                                index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) 
                                    < fxPtMin
                                ) ? fxPtMin
                                : index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y);
                        }
                    }
                }
            }
        }
    }

}


template class PoolingLayer<float>;
template class PoolingLayer<FixedPoint>;

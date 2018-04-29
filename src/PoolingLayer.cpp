#include "PoolingLayer.hpp"
using namespace std;



PoolingLayer::PoolingLayer   (
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


PoolingLayer::~PoolingLayer() {
    if(m_precision == FLOAT) {
        free(m_blob.flData);
    } else {
        free(m_blob.fxData);
    }
}


void PoolingLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

    // output size
    m_outputDepth = m_inputDepth;
    if(m_globalPooling) {
        m_numOutputRows = 1;
        m_numOutputCols = 1;
    } else {
        m_numOutputRows = int(ceil(float(m_numInputRows - m_numKernelRows + 2.0f * m_padding) / float(m_stride))) + 1;
        m_numOutputCols = int(ceil(float(m_numInputCols - m_numKernelCols + 2.0f * m_padding) / float(m_stride))) + 1;
    }

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    if(m_precision == FLOAT) {
        m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    } else {
        m_blob.fxData = (FixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(FixedPoint_t));
    }  
}


void PoolingLayer::ComputeLayer() {

    if(m_precision == FLOAT) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------       
        // get input
        float *datain = m_bottomLayers[0]->m_blob.flData;
        int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
        int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
        int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

        // output
        float *dataout = m_topLayers[0]->m_blob.flData;     
        float minValue = -FLT_MAX;
      
        if(m_globalPooling) {
            if(m_layerType == "Pooling_MAX") {
                // TODO
            } else if(m_layerType == "Pooling_AVE") { 
                // TODO
            }
        } else {
            if(m_layerType == "Pooling_MAX") {
                for (int m = 0; m < m_outputDepth; m++) {
                    for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
                        for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = minValue;
                            for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                                for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                                    if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                        index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                                < index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                            ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                            : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                                    } else {
                                        index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                                < minValue
                                            ) ? minValue
                                            : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if(m_layerType == "Pooling_AVE") { 
                for (int m = 0; m < m_outputDepth; m++) {
                    for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
                        for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
                            float ave = 0.0f;
                            for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                                for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                                    if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                        ave += index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j);
                                    }
                                }
                            }
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = ave / float(m_numKernelRows * m_numKernelCols);
                        }
                    }
                }
            }
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------        
        
    } else {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------       
        // get input
        FixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
        int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
        int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
        int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

        // output
        FixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;        
        FixedPoint_t minValue = 0x8000000000000000;
            
        if(m_globalPooling) {
            if(m_layerType == "Pooling_MAX") {
                // TODO
            } else if(m_layerType == "Pooling_AVE") { 
                // TODO
            }
        } else {
            if(m_layerType == "Pooling_MAX") {
                for (int m = 0; m < m_outputDepth; m++) {
                    for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
                        for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = minValue;
                            for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                                for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                                    if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                        index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                                < index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                            ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                            : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                                    } else {
                                        index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                                < minValue
                                            ) ? minValue
                                            : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if(m_layerType == "Pooling_AVE") { 
                for (int m = 0; m < m_outputDepth; m++) {
                    for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
                        for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
                            FixedPoint_t ave = 0;
                            for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                                for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                                    if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                                        ave += index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j);
                                    }
                                }
                            }
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = ave / FixedPoint_t(m_numKernelRows * m_numKernelCols);
                        }
                    }
                }
            }
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------                
        
    }
}

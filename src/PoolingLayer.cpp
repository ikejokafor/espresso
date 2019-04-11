#include "PoolingLayer.hpp"
using namespace std;
using namespace espresso;


PoolingLayer::PoolingLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


PoolingLayer::~PoolingLayer() {}


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
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}


void PoolingLayer::ComputeLayer() {
    if(m_precision == FLOAT) {
	    ComputeLayer_FlPt();
    } else if(m_precision == FIXED) {
	    ComputeLayer_FxPt();
    }   
}


void PoolingLayer::ComputeLayer_FlPt() {
    if(m_globalPooling) {
        if(m_layerType == "Pooling_MAX") {
	        MaxGlobalPooling_FlPt();
        } else if (m_layerType == "Pooling_AVE") { 
	        AvgGlobalPooling_FlPt();
        }
    } else {
        if(m_layerType == "Pooling_MAX") {
	        MaxPooling_FlPt();
        } else if(m_layerType == "Pooling_AVE") { 
	        AvgPooling_FlPt();
        }
	}
}


void PoolingLayer::MaxGlobalPooling_FlPt() {}


void PoolingLayer::AvgGlobalPooling_FlPt() {}


void PoolingLayer::MaxPooling_FlPt() {
	if(m_bottomLayers[0]->m_precision == FIXED) {
		int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for (int i = 0; i < blobSize; i++) {
			flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
		}
	}

        
	// get input
	float *datain = m_bottomLayers[0]->m_blob.flData;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.flData;     
	float minValue = -FLT_MAX;
	
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
}

void PoolingLayer::AvgPooling_FlPt() {
    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }

        
    // get input
    float *datain = m_bottomLayers[0]->m_blob.flData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

    // output
    float *dataout = m_topLayers[0]->m_blob.flData;     
    float minValue = -FLT_MAX;
	
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

	
void PoolingLayer::ComputeLayer_FxPt() {
    if(m_globalPooling) {
        if(m_layerType == "Pooling_MAX") {
	        MaxGlobalPooling_FxPt();
        } else if (m_layerType == "Pooling_AVE") { 
	        AvgGlobalPooling_FxPt();
        }
    } else {
        if(m_layerType == "Pooling_MAX") {
	        MaxPooling_FxPt();
        } else if(m_layerType == "Pooling_AVE") { 
	        AvgPooling_FxPt();
        }
	}
}


void PoolingLayer::MaxGlobalPooling_FxPt() {}


void PoolingLayer::AvgGlobalPooling_FxPt() {}


void PoolingLayer::MaxPooling_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
        }
    }
        
    if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength){
        fixedPoint::SetParam(   m_bottomLayers[0]->m_doutFxPtLength, 
                                m_bottomLayers[0]->m_doutNumFracBits, 
                                m_dinFxPtLength, 
                                m_dinNumFracBits, 
                                m_bottomLayers[0]->m_blob.fxData,
                                m_bottomLayers[0]->m_blob.blobSize
                            );
    }
        
    // get input
    fixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;        
    fixedPoint_t minValue = 0x8000000000000000;

	
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
	
	
	fixedPoint::SetParam(   m_dinFxPtLength,
                        m_dinNumFracBits, 
                        m_doutFxPtLength, 
                        m_doutNumFracBits, 
                        m_topLayers[0]->m_blob.fxData,
                        m_topLayers[0]->m_blob.blobSize
                    );
}


void PoolingLayer::AvgPooling_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
        }
    }
        
    if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength){
        fixedPoint::SetParam(   m_bottomLayers[0]->m_doutFxPtLength, 
                                m_bottomLayers[0]->m_doutNumFracBits, 
                                m_dinFxPtLength, 
                                m_dinNumFracBits, 
                                m_bottomLayers[0]->m_blob.fxData,
                                m_bottomLayers[0]->m_blob.blobSize
                            );
    }
        
    // get input
    fixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	
    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;        
	
	
    for (int m = 0; m < m_outputDepth; m++) {
        for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
            for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
                fixedPoint_t ave = 0;
                for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                    for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                        if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                            ave += index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j);
                        }
                    }
                }
                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = ave / fixedPoint_t(m_numKernelRows * m_numKernelCols);
            }
        }
    }
	
	
	fixedPoint::SetParam(   m_dinFxPtLength,
						m_dinNumFracBits, 
						m_doutFxPtLength, 
						m_doutNumFracBits, 
						m_topLayers[0]->m_blob.fxData,
						m_topLayers[0]->m_blob.blobSize
					);

}
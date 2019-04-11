#include "FullyConnectedLayer.hpp"
using namespace std;
using namespace espresso;


FullyConnectedLayer::FullyConnectedLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


FullyConnectedLayer::~FullyConnectedLayer() {
	if(m_blob.flData) {
		free(m_blob.flData);
	}
	if(m_blob.fxData) {
		free(m_blob.fxData);
	}
}


void FullyConnectedLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_numOutputRows = 1;
	m_numOutputCols = 1;
    
    // kernel depth
    m_kernelDepth = m_inputDepth * m_numInputRows * m_numInputCols;
    m_numKernels = m_outputDepth;
    
	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}


void FullyConnectedLayer::ComputeLayer() {
    if(m_precision == FLOAT) {
	    ComputeLayer_FlPt();
    } else if(m_precision == FIXED) {
        ComputeLayer_FxPt();
    }
}


void FullyConnectedLayer::ComputeLayer_FlPt() {
    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }
	
	if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength || m_bottomLayers[0]->m_doutNumFracBits != m_dinNumFracBits) {
		fixedPoint::SetParam(   m_bottomLayers[0]->m_doutFxPtLength, 
								m_bottomLayers[0]->m_doutNumFracBits, 
								m_dinFxPtLength, 
								m_dinNumFracBits, 
								m_bottomLayers[0]->m_blob.fxData,
								m_bottomLayers[0]->m_blob.blobSize
							);
	}
        
    // get input
    float *datain = m_bottomLayers[0]->m_blob.flData;
        
    // output
    float *dataout = m_topLayers[0]->m_blob.flData;
        
    for (int m = 0; m < m_numKernels; m++) {
        dataout[m] = m_flBiasData[m];
        for (int k = 0; k < m_kernelDepth; k++) {
            dataout[m] += datain[k] * index2D(m_numKernels, m_kernelDepth, m_flFilterData, m, k);
        }
    }  
}



void FullyConnectedLayer::ComputeLayer_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
        }
    }
        
    if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength || m_bottomLayers[0]->m_doutNumFracBits != m_dinNumFracBits) {
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
        
    //filter data
	fixedPoint_t *filters = m_fxFilterData;
        
    int resFxPtLength  = m_dinFxPtLength + m_whtFxPtLength;
    int resNumFracBits = m_dinNumFracBits + m_whtNumFracBits;
        
    fixedPoint_t *bias = (fixedPoint_t*)malloc(sizeof(fixedPoint_t) * m_numKernels);
    memcpy(bias, m_fxBiasData, (sizeof(fixedPoint_t) * m_numKernels));
    fixedPoint::SetParam(   m_biasFxPtLength,
                            m_biasNumFracBits,
                            resFxPtLength,
                            resNumFracBits,
                            bias,
                            m_numKernels
                        );
  
    for (int m = 0; m < m_numKernels; m++) {
        dataout[m] = bias[m];
        for (int k = 0; k < m_kernelDepth; k++) {
            dataout[m] += datain[k] * index2D(m_numKernels, m_kernelDepth, filters, m, k);
        }
    }
        

    fixedPoint::SetParam(   resFxPtLength, 
                            resNumFracBits, 
                            m_doutFxPtLength, 
                            m_doutNumFracBits, 
                            m_topLayers[0]->m_blob.fxData,
                            m_topLayers[0]->m_blob.blobSize
                        );
                            
    free(bias);
}

#include "UpSampleLayer.hpp"
using namespace std;
using namespace espresso;


UpSampleLayer::UpSampleLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


UpSampleLayer::~UpSampleLayer() {}


void UpSampleLayer::ComputeLayerParam() {
	// input size
	m_inputDepth   = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_inputDepth;
	m_numOutputRows = m_numInputRows * m_stride;
	m_numOutputCols = m_numInputCols * m_stride;
    
	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}


void UpSampleLayer::ComputeLayer() {
	if (m_precision == FLOAT) {  
		ComputeLayer_FlPt();
	} else if(m_precision == FIXED) { 
		ComputeLayer_FxPt();
	}
}


void UpSampleLayer::ComputeLayer_FlPt() {
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
	
	
	for(int k = 0; k < inputBlobDepth; ++k){
		for(int j = 0; j < numInputBlobRows * m_stride; ++j){
			for(int i = 0; i < numInputBlobCols * m_stride; ++i){
				int in_index = k * numInputBlobCols * numInputBlobRows + (j / m_stride) * numInputBlobCols + i/m_stride;
				int out_index = k * numInputBlobCols * numInputBlobRows * m_stride * m_stride + j * numInputBlobCols * m_stride + i;
				dataout[out_index] = 1.0f * datain[in_index];
			}
		}
	}
}


void UpSampleLayer::ComputeLayer_FxPt() {
	if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
        }
    }
        
    // get input
    fixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;
        
    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
	
	
	for(int k = 0; k < inputBlobDepth; ++k){
		for(int j = 0; j < numInputBlobRows * m_stride; ++j){
			for(int i = 0; i < numInputBlobCols * m_stride; ++i){
				int in_index = k * numInputBlobCols * numInputBlobRows + (j / m_stride) * numInputBlobCols + i / m_stride;
				int out_index = k * numInputBlobCols * numInputBlobRows * m_stride * m_stride + j * numInputBlobCols * m_stride + i;
				dataout[out_index] = 1.0f * datain[in_index];
			}
		}
	}
}

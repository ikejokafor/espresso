#include "ConcatLayer.hpp"
using namespace std;
using namespace espresso;

ConcatLayer::ConcatLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


ConcatLayer::~ConcatLayer() {
	if(m_blob.flData) {
		free(m_blob.flData);
	}
	if(m_blob.fxData) {
		free(m_blob.fxData);
	}
}


void ConcatLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth	= m_bottomLayers[0]->m_outputDepth;
	for (uint32_t i = 1; i < m_bottomLayers.size(); i++) {
		m_outputDepth += m_bottomLayers[i]->m_outputDepth;
	}

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}



void ConcatLayer::ComputeLayer() {
    if(m_precision == FLOAT) {
		ComputeLayer_FlPt();
    } else if(m_precision == FIXED) {
	    ComputeLayer_FxPt();
    }
}


void ConcatLayer::ComputeLayer_FlPt() {
    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }
        
    // output
    float *dataout = m_topLayers[0]->m_blob.flData;

    for (uint32_t i = 0; i < m_bottomLayers.size(); i++) {
        memcpy(dataout, m_bottomLayers[i]->m_blob.flData, m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols * sizeof(float));
        dataout += (m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols);
    }
}


void ConcatLayer::ComputeLayer_FxPt() {
	if(m_bottomLayers[0]->m_precision == FLOAT) {
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for(int i = 0; i < blobSize; i++) {
			fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
		}
	}

        
	// output
	fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
        
	for (uint32_t i = 0; i < m_bottomLayers.size(); i++) {
		memcpy(dataout, m_bottomLayers[i]->m_blob.fxData, m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols * sizeof(fixedPoint_t));
		dataout += (m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols);
	}
}




#include "ConcatLayer.hpp"


ConcatLayer::ConcatLayer(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


ConcatLayer::~ConcatLayer() { }


void ConcatLayer::ComputeLayer()
{
	if (m_precision == espresso::FLOAT) {
		ComputeLayer_FlPt();
    }
	else if(m_precision == espresso::FIXED) {
	    ComputeLayer_FxPt();
    }
}


void ConcatLayer::ComputeLayer_FlPt()
{
	if (m_bottomLayers[0]->m_precision == espresso::FIXED) {
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


void ConcatLayer::ComputeLayer_FxPt()
{
	if (m_bottomLayers[0]->m_precision == espresso::FLOAT)
	{
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for(int i = 0; i < blobSize; i++) {
			fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
		}
	}
	// output
	fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
	for (uint32_t i = 0; i < m_bottomLayers.size(); i++)
	{
		memcpy(dataout, m_bottomLayers[i]->m_blob.fxData, m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols * sizeof(fixedPoint_t));
		dataout += (m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols);
	}
}


void ConcatLayer::ComputeLayerParam()
{
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
	m_blob.flData = new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}


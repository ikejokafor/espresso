#include "UpSampleLayer_DKN.hpp"


UpSampleLayer_DKN::UpSampleLayer_DKN(espresso::layerInfo_obj layerInfo) : Layer(layerInfo) { }


UpSampleLayer_DKN::~UpSampleLayer_DKN() { }


void UpSampleLayer_DKN::ComputeLayer() 
{
	if (m_bottomLayers[0]->m_precision == espresso::FIXED) 
	{
		int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for (int i = 0; i < blobSize; i++) 
		{
			flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
		}
	}
	m_yolo_net->input = m_bottomLayers[0]->m_blob.flData;
	layer l = m_yolo_net->layers[m_net_idx];
	l.forward(l, m_yolo_net[0]);
	m_topLayers[0]->m_blob.flData = l.output;
}


void UpSampleLayer_DKN::ComputeLayer_FlPt() { }


void UpSampleLayer_DKN::ComputeLayer_FxPt() { }


void UpSampleLayer_DKN::ComputeLayerParam() 
{
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
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

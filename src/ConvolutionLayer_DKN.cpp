#include "ConvolutionLayer_DKN.hpp"


ConvolutionLayer_DKN::ConvolutionLayer_DKN(espresso::layerInfo_obj layerInfo) : Layer(layerInfo) { }


ConvolutionLayer_DKN::~ConvolutionLayer_DKN() { }


void ConvolutionLayer_DKN::ComputeLayer() 
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


void ConvolutionLayer_DKN::ComputeLayer_FlPt() { }


void ConvolutionLayer_DKN::ComputeLayer_FxPt() { }


void ConvolutionLayer_DKN::ComputeLayerParam() 
{
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;
	m_numOutputRows = (int)((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = (int)((m_numInputCols - m_numKernelCols + 2 * m_padding) / m_stride) + 1;
	m_numKernels = m_outputDepth;
	m_kernelDepth = m_inputDepth / m_group;
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData =  new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}




#include "ConcatLayer_FPGA.hpp"
using namespace std;


ConcatLayer_FPGA::ConcatLayer_FPGA(espresso::layerInfo_obj layerInfo) : Layer(layerInfo)  { }


ConcatLayer_FPGA::~ConcatLayer_FPGA() { }


void ConcatLayer_FPGA::ComputeLayer() 
{
	if (m_bottomLayers[0]->m_precision == espresso::FLOAT) 
	{
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for (int i = 0; i < blobSize; i++) 
		{
			fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
		}
	}
	cout << m_layerName << "Merged" <<  endl;
}


void ConcatLayer_FPGA::ComputeLayer_FlPt() { }


void ConcatLayer_FPGA::ComputeLayer_FxPt() { }


void ConcatLayer_FPGA::ComputeLayerParam() 
{
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;
	// output size;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth = m_bottomLayers[0]->m_outputDepth;
	for (uint32_t i = 1; i < m_bottomLayers.size(); i++) 
	{
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


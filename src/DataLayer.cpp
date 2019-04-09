#include "DataLayer.hpp"
using namespace std;
using namespace espresso;


DataLayer::DataLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


DataLayer::~DataLayer() {}


void DataLayer::ComputeLayerParam() {
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth = m_inputDepth;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
}


void DataLayer::ComputeLayer() {}


void DataLayer::ComputeLayer_FlPt() {}


void DataLayer::ComputeLayer_FxPt() {}

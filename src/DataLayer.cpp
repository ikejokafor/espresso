#include "DataLayer.hpp"
using namespace std;

DataLayer::DataLayer	(
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
							float *filterData,
							float *biasData
						) : Layer	(	
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
											filterData,
											biasData
										) {
}


DataLayer::~DataLayer() {}


void DataLayer::ComputeLayerParam() {
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth = m_inputDepth;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
}


void DataLayer::ComputeLayer() {

}
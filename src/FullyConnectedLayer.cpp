#include "FullyConnectedLayer.hpp"
using namespace std;

FullyConnectedLayer::FullyConnectedLayer	(
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


FullyConnectedLayer::~FullyConnectedLayer() {}


void Layer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_numOutputRows = m_inputDepth * m_numInputRows * m_numInputCols;
	m_numOutputCols = 1;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numKernelCols;
	m_blob.data = (float*)malloc(m_outputDepth * m_numOutputRows * m_numKernelCols * sizeof(float));
}

void FullyConnectedLayer::ComputeLayer(Blob_t inputBlob) {

	// get input
	float *datain = m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.data;

	for (int m = 0; m < m_numKernels; m++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			index2D(m_outputDepth, m_numOutputRows, dataout, m, x) = m_biasData[m];
			for (int k = 0; k < m_kernelDepth; k++) {
				index2D(m_outputDepth, m_numOutputRows, dataout, m, x) += datain[k] * index2D(m_numKernels, m_kernelDepth, m_filterData, m, k);
			}
		}
	}

}
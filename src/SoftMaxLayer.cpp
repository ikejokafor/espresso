#include "SoftMaxLayer.hpp"
using namespace std;

SoftMaxLayer::SoftMaxLayer	(
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


SoftMaxLayer::~SoftMaxLayer() {}


void Layer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth   = m_inputDepth;
	m_numOutputCols = m_numInputRows;
	m_numOutputRows = m_numInputCols;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numKernelCols;
	m_blob.data = (float*)malloc(m_outputDepth * m_numOutputRows * m_numKernelCols * sizeof(float));
}

void SoftMaxLayer::ComputeLayer(Blob_t inputBlob) {

	// get input
	float *datain = m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.data;


	float sum = 0;
	for (int i = 0; i < inputBlobDepth; i++) {
		dataout[i] = exp(datain[i]);
		sum += (dataout[i]);
	}
	for (int i = 0; i < inputBlobDepth; i++) {
		dataout[i] = dataout[i] / sum;
	}

}
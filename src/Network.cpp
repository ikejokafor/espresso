#include "Network.hpp"
using namespace std;

Network::Network(vector<layerInfo_t> &layerInfo) {
	
	for (int i = 0; i < layerInfo.size(); i++) {
		if (layerInfo[i].layerType == "Input") {
			m_cnn.push_back(new DataLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth
			));
		} else if(layerInfo[i].layerType == "Convolution") {
			m_cnn.push_back(new ConvolutionLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
				layerInfo[i].filterData,
				layerInfo[i].biasData
			));
		} else if(layerInfo[i].layerType == "ReLU") {
			m_cnn.push_back(new RELULayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
				layerInfo[i].filterData,
				layerInfo[i].biasData
			));
		} else if(layerInfo[i].layerType == "Pooling") {
			m_cnn.push_back(new PoolingLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding
			));
		} else if(layerInfo[i].layerType == "InnerProduct") {
			m_cnn.push_back(new FullyConnectedLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
				layerInfo[i].filterData,
				layerInfo[i].biasData
			));
		} else if(layerInfo[i].layerType == "Concat") {
			m_cnn.push_back(new ConcatLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType
			));
		} else if(layerInfo[i].layerType == "SoftMax") {
			m_cnn.push_back(new SoftMaxLayer(
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType
			));
		}
	}

	for (int i = 0; i < m_cnn.size(); i++) {	// for every layer
		if (m_cnn[i]->m_layerType != "Input") {
			for (int j = 0; j < m_cnn[i]->m_topLayerNames.size(); j++) { // for every top layer of the current cnn layer
				for (int k = 0; k < m_cnn.size(); k++) {	// search for the top layer 
					if (m_cnn[i]->m_topLayerNames[j] == m_cnn[k]->m_layerName) {
						m_cnn[i]->m_topLayers.push_back(m_cnn[k]);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < m_cnn.size(); i++) {	// for every layer
		if (m_cnn[i]->m_layerType != "Input") {
			for (int j = 0; j < m_cnn[i]->m_bottomLayerNames.size(); j++) { // for every bottom layer of the current cnn layer
				for (int k = 0; k < m_cnn.size(); k++) {	// search for the top layer
					if (m_cnn[i]->m_bottomLayerNames[j] == m_cnn[k]->m_layerName) {
						m_cnn[i]->m_bottomLayers.push_back(m_cnn[k]);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < m_cnn.size(); i++) {
		m_cnn[i]->ComputeLayerParam();
	}
}


Network::~Network() {
	for (int i = 0; i < m_cnn.size(); i++) {
		delete m_cnn[i];
	}
}


void Network::Forward(Blob_t inputBlob, Blob_t *outputBlob) {
	
	for (int i = 0; i < m_cnn.size(); i++) {
		m_cnn[i]->ComputeLayer(inputBlob);
	}

	outputBlob->data	= m_cnn[m_cnn.size() - 1]->m_blob.data;
	outputBlob->numRows = m_cnn[m_cnn.size() - 1]->m_blob.numRows;
	outputBlob->numCols = m_cnn[m_cnn.size() - 1]->m_blob.numCols;
	outputBlob->depth	= m_cnn[m_cnn.size() - 1]->m_blob.depth;
}
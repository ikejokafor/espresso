#include "RELULayer.hpp"
using namespace std;

RELULayer::RELULayer	(
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

RELULayer::~RELULayer() {}


void Layer::ComputeLayerParam() {
	// input size
	m_inputDepth   = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_inputDepth;
	m_numOutputCols = m_numInputRows;
	m_numOutputRows = m_numInputCols;
	}


void RELULayer::ComputeLayer(Blob_t inputBlob) {

	// get input
	float *datain = m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.data;


	for (int m = 0; m < m_outputDepth; m++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			for(int y = 0; y < m_numOutputCols; y++) {
				index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
					index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0
				) ? 0 : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, datain, m, x, y);
			}
		}
	}

}

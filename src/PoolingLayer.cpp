#include "PoolingLayer.hpp"
using namespace std;

PoolingLayer::PoolingLayer	(
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


PoolingLayer::~PoolingLayer() {}


void PoolingLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_numOutputRows = ((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = ((m_numInputCols - m_numOutputCols + 2 * m_padding) / m_stride) + 1;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numKernelCols;
	m_blob.data = (float*)malloc(m_outputDepth * m_numOutputRows * m_numKernelCols * sizeof(float));
}

void PoolingLayer::ComputeLayer(Blob_t inputBlob) {

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
				index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = -FLT_MAX;
				for (int i = -m_padding; i < m_numKernelRows; i++) {
					for (int j = -m_padding; j < m_numKernelCols; j++) {
						for (int k = 0; k < inputBlobDepth; k++) {
							if (i >= 0 && j >= 0) {	// in valid region, 
								index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
										index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) <
										index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, (m_stride * x + i), (m_stride * y + j))
								)	? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, (m_stride * x + i), (m_stride * y + j))
									: index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
							} else {
								index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
									index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) <
									-FLT_MAX
									) ? -FLT_MAX
									: index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
							}
						}
					}
				}
			}
		}
	}

}
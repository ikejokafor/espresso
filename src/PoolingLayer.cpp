#include "PoolingLayer.hpp"
using namespace std;

PoolingLayer::PoolingLayer(
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
) : Layer(
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


PoolingLayer::~PoolingLayer() {
    free(m_blob.data);
}


void PoolingLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_inputDepth;
	m_numOutputRows = (int)ceil((float)(m_numInputRows - m_numKernelRows + 2 * m_padding) / (float)m_stride) + 1;
	m_numOutputCols = (int)ceil((float)(m_numInputCols - m_numKernelCols + 2 * m_padding) / (float)m_stride) + 1;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.data = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
}

void PoolingLayer::ComputeLayer() {

	// get input
	float *datain = m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.data;
    
	for (int m = 0; m < m_outputDepth; m++) {
		for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
			for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
				index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = -FLT_MAX;
                for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
                    for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
                        if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                    < index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, i, j)
                                : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                        } else {
                            index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                                    < -FLT_MAX
                                ) ? -FLT_MAX
                                : index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
                        }
                    }
                }
			}
		}
	}

}
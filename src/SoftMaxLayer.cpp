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


SoftMaxLayer::~SoftMaxLayer() {
    free(m_blob.data);
}


void SoftMaxLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth   = m_inputDepth;
	m_numOutputCols = m_numInputCols;
	m_numOutputRows = m_numInputRows;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.data = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
}

void SoftMaxLayer::ComputeLayer() { // made with dcNet in mind, may need to change for general Neural networks with common softmax layers

	// get input
	float *datain = m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

	// output
	float *dataout = m_topLayers[0]->m_blob.data;
    
    // get max
    float *maxs = (float*)malloc(m_numOutputRows * m_numOutputCols * sizeof(float));
    for (int x = 0; x < m_numOutputRows; x++) {
        for(int y = 0; y < m_numOutputCols; y++) {
            index2D(m_numOutputRows, m_numOutputCols, maxs, x, y) = -FLT_MAX;
        }
    }
    for (int m = 0; m < m_outputDepth; m++) {
        for (int x = 0; x < m_numOutputRows; x++) {
            for(int y = 0; y < m_numOutputCols; y++) {
                index2D(m_numOutputRows, m_numOutputCols, maxs, x, y) = (
                    index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) 
                    > index2D(m_numOutputRows, m_numOutputCols, maxs, x, y) 
                ) ? index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) 
                  : index2D(m_numOutputRows, m_numOutputCols, maxs, x, y);
			}
		}
	}
    
    
    // sub, exp, and sums
    float *sums = (float*)malloc(m_numOutputRows * m_numOutputCols * sizeof(float));
    for (int x = 0; x < m_numOutputRows; x++) {
        for(int y = 0; y < m_numOutputCols; y++) {
            index2D(m_numOutputRows, m_numOutputCols, sums, x, y) = 0.0f;
        }
    }
	for (int m = 0; m < m_outputDepth; m++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			for(int y = 0; y < m_numOutputCols; y++) {
                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = 
                    exp(index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) - index2D(m_numOutputRows, m_numOutputCols, maxs, x, y));
                index2D(m_numOutputRows, m_numOutputCols, sums, x, y) += index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y);
			}
		}
	}
    
    // division
    for (int m = 0; m < m_outputDepth; m++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			for(int y = 0; y < m_numOutputCols; y++) {
                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) /= index2D(m_numOutputRows, m_numOutputCols, sums, x, y);
			}
		}
	}

    free(maxs);
    free(sums);

}
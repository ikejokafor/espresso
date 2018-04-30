#include "NormLayer.hpp"
using namespace std;


NormLayer::NormLayer    (
                                precision_t precision,
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
                                bool globalPooling,
                                float *flFilterData,
                                float *flBiasData,  
                                FixedPoint_t *fxFilterData,
                                FixedPoint_t *fxBiasData,                                    
                                int group,
                                int localSize,
                                float alpha,
                                float beta,
                                int dinFxPtLength,
                                int dinNumFracBits,
                                int whtFxPtLength,
                                int whtNumFracBits,
                                int doutFxPtLength,
                                int doutNumFracBits
                            ) : Layer	(	
                                            precision,
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
                                            globalPooling,
                                            flFilterData,
                                            flBiasData,                                                          
                                            fxFilterData,                                                        
                                            fxBiasData,                                                          
                                            group,
                                            localSize,
                                            alpha,
                                            beta,
                                            dinFxPtLength,
                                            dinNumFracBits,
                                            whtFxPtLength,
                                            whtNumFracBits,
                                            doutFxPtLength,
                                            doutNumFracBits
                                        ) {
}


NormLayer::~NormLayer() {
    free(m_blob.flData); 
    free(m_blob.fxData);
}


void NormLayer::ComputeLayerParam() {
	// input size
	m_inputDepth   = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_inputDepth;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
    
    // create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (FixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(FixedPoint_t));
}


void NormLayer::ComputeLayer() {

    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        FixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = FixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }

	// get input
	float *datain = m_bottomLayers[0]->m_blob.flData;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

    
	// output
	float *dataout = m_topLayers[0]->m_blob.flData;
 
 
    // get padded x_i squared
    float *paddedSquare = (float*)malloc((m_inputDepth + m_localSize - 1) * m_numOutputRows * m_numOutputCols * sizeof(float));
    memset(paddedSquare, 0, ((m_inputDepth + m_localSize - 1) * m_numOutputRows * m_numOutputCols * sizeof(float)));
    float *pre_pad_offset = paddedSquare + (((m_localSize - 1) / 2) * m_numOutputRows * m_numOutputCols);
    memcpy(pre_pad_offset, datain, (m_inputDepth * m_numOutputRows * m_numOutputCols * sizeof(float)));
    for(int i = 0; i < ((m_inputDepth + m_localSize - 1) * m_numOutputRows * m_numOutputCols); i++) {   // dont really need to square the pad values, because they are zero
        paddedSquare[i] = paddedSquare[i] * paddedSquare[i];
    }
    

    // compute norm values
    float *normValues = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    int paddedSquareDepth = (m_inputDepth + m_localSize - 1);
	for (int m = 0, start = 0; m < m_outputDepth; m++, start++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			for(int y = 0; y < m_numOutputCols; y++) {
                float sum = 0.0f;
                for(int i = start; i < (m_localSize + start); i++) {
                    sum += index3D(paddedSquareDepth, numInputBlobRows, numInputBlobCols, paddedSquare, i, x, y);
                }
                index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, normValues, m, x, y) = pow((1.0f + (m_alpha / (float)m_localSize) * sum), m_beta);
			}
		}
	}
    
   
    // normalize
	for (int m = 0; m < m_outputDepth; m++) {
		for (int x = 0; x < m_numOutputRows; x++) {
			for(int y = 0; y < m_numOutputCols; y++) {
				index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) 
                    = index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y)
                        / index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, normValues, m, x, y);
			}
		}
	}
}

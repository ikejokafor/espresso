#include "ConcatLayer.hpp"
using namespace std;


ConcatLayer::ConcatLayer    (
                                precision_t precision,
                                string layerName,
                                vector<string> topLayerNames,
                                vector<string> bottomLayerNames,
                                string layerType,
                                int numInputRows,
                                int numInputCols,
                                int inputDepth,
                                int outputDepth,
                                int dinFxPtLength,
                                int dinNumFracBits,
                                int whtFxPtLength,
                                int whtNumFracBits,
                                int doutFxPtLength,
                                int doutNumFracBits,                                
                                int numKernelRows,
                                int numKernelCols,
                                int stride,
                                int padding,
                                bool globalPooling,
                                float *flFilterData,
                                float *flBiasData,  
                                FixedPoint_t *fxFilterData,
                                FixedPoint_t *fxBiasData, 
                                int numFilterValues,                                
                                int group,
                                int localSize,
                                float alpha,
                                float beta
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
                                            dinFxPtLength,
                                            dinNumFracBits,
                                            whtFxPtLength,
                                            whtNumFracBits,
                                            doutFxPtLength,
                                            doutNumFracBits,                                            
                                            numKernelRows,
                                            numKernelCols,
                                            stride,
                                            padding,
                                            globalPooling,
                                            flFilterData,
                                            flBiasData,                                                          
                                            fxFilterData,                                                        
                                            fxBiasData, 
                                            numFilterValues,
                                            group,
                                            localSize,
                                            alpha,
                                            beta
                                        ) {
}


ConcatLayer::~ConcatLayer() {
    free(m_blob.flData);
    free(m_blob.fxData);
}


void ConcatLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth	= m_bottomLayers[0]->m_outputDepth;
	for (uint32_t i = 1; i < m_bottomLayers.size(); i++) {
		m_outputDepth += m_bottomLayers[i]->m_outputDepth;
	}

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (FixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(FixedPoint_t));
}



void ConcatLayer::ComputeLayer() {

    if(m_precision == FLOAT) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------  
        if(m_bottomLayers[0]->m_precision == FIXED) {
            int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
            int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
            FixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
            float        *flData = m_bottomLayers[0]->m_blob.flData;
            for(int i = 0; i < blobSize; i++) {
                flData[i] = FixedPoint::toFloat(dinNumFracBits, fxData[i]);
            }
        }
        
        // output
        float *dataout = m_topLayers[0]->m_blob.flData;

        for (uint32_t i = 0; i < m_bottomLayers.size(); i++) {
            memcpy(dataout, m_bottomLayers[i]->m_blob.flData, m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols * sizeof(float));
            dataout += (m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols);
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------

    } else if(m_precision == FIXED) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------
        if(m_bottomLayers[0]->m_precision == FLOAT) {
            int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
            FixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
            float        *flData = m_bottomLayers[0]->m_blob.flData;
            for(int i = 0; i < blobSize; i++) {
                fxData[i] = FixedPoint::create(m_dinNumFracBits, flData[i]);
            }
        }

        
        // output
        FixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
        
        for (uint32_t i = 0; i < m_bottomLayers.size(); i++) {
            memcpy(dataout, m_bottomLayers[i]->m_blob.fxData, m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols * sizeof(FixedPoint_t));
            dataout += (m_bottomLayers[i]->m_blob.depth * m_bottomLayers[i]->m_blob.numRows * m_bottomLayers[i]->m_blob.numCols);
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------
        
    }
 
}

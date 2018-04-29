#include "FullyConnectedLayer.hpp"
using namespace std;


FullyConnectedLayer::FullyConnectedLayer    (
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
                                                int whtNumFracBits
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
                                                        whtNumFracBits
                                                    ) {
}


FullyConnectedLayer::~FullyConnectedLayer() {
    if(m_precision == FLOAT) {
        free(m_blob.flData);
    } else {
        free(m_blob.fxData);
    }
}


void FullyConnectedLayer::ComputeLayerParam() {
	// input size
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_numOutputRows = 1;
	m_numOutputCols = 1;
    
    // kernel depth
    m_kernelDepth = m_inputDepth * m_numInputRows * m_numInputCols;
    m_numKernels = m_outputDepth;
    
	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    if(m_precision == FLOAT) {
        m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    } else {
        m_blob.fxData = (FixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(FixedPoint_t));
    }
}


void FullyConnectedLayer::ComputeLayer() {

    if(m_precision == FLOAT) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------        
        // get input
        float *datain = m_bottomLayers[0]->m_blob.flData;
        
        // output
        float *dataout = m_topLayers[0]->m_blob.flData;
        
        for (int m = 0; m < m_numKernels; m++) {
            dataout[m] = m_flBiasData[m];
            for (int k = 0; k < m_kernelDepth; k++) {
                dataout[m] += datain[k] * index2D(m_numKernels, m_kernelDepth, m_flFilterData, m, k);
            }
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------        

    } else {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------               
        // get input
        FixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
        
        // output
        FixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
        
        for (int m = 0; m < m_numKernels; m++) {
            dataout[m] = m_fxBiasData[m];
            for (int k = 0; k < m_kernelDepth; k++) {
                dataout[m] += datain[k] * index2D(m_numKernels, m_kernelDepth, m_fxFilterData, m, k);
            }
        } 
        // End Code ---------------------------------------------------------------------------------------------------------------------------------        
        
    }
}

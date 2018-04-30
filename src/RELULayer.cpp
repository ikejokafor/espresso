#include "RELULayer.hpp"
using namespace std;


RELULayer::RELULayer    (
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


RELULayer::~RELULayer() {}


void RELULayer::ComputeLayerParam() {
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
    m_blob.flData = (float*)m_topLayers[0]->m_blob.flData;
    m_blob.fxData = (FixedPoint_t*)m_topLayers[0]->m_blob.fxData; 
}


void RELULayer::ComputeLayer() {

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
        
        // get input
        float *datain = m_bottomLayers[0]->m_blob.flData;
        int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
        int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
        int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

        // output
        float *dataout = m_topLayers[0]->m_blob.flData;
       
        for (int m = 0; m < m_outputDepth; m++) {
            for (int x = 0; x < m_numOutputRows; x++) {
                for(int y = 0; y < m_numOutputCols; y++) {
                    index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                        index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0 
                    ) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
                }
            }
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
        
        // get input
        FixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
        int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
        int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
        int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;

        // output
        FixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
  
        for (int m = 0; m < m_outputDepth; m++) {
            for (int x = 0; x < m_numOutputRows; x++) {
                for(int y = 0; y < m_numOutputCols; y++) {
                    index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                        index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0 
                    ) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
                }
            }
        }
        // End Code ---------------------------------------------------------------------------------------------------------------------------------     
        
    }   
}

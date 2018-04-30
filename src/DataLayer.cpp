#include "DataLayer.hpp"
using namespace std;


DataLayer::DataLayer	(
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



DataLayer::~DataLayer() {}


void DataLayer::ComputeLayerParam() {
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
	m_outputDepth = m_inputDepth;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
}


void DataLayer::ComputeLayer() {}

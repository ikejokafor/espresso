#include "DataLayer.hpp"
using namespace std;


template <typename DType>
DataLayer<DType>::DataLayer	(
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
                                    DType *filterData,
                                    DType *biasData,
                                    int group,
                                    int localSize,
                                    float alpha,
                                    float beta,
                                    int length,
                                    int numFracbits
                                ) : Layer<DType>	(	
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
                                                        filterData,
                                                        biasData,
                                                        group,
                                                        localSize,
                                                        alpha,
                                                        beta,
                                                        length,
                                                        numFracbits
                                                    ) {
}


template <typename DType>
DataLayer<DType>::~DataLayer() {}


template <typename DType>
void DataLayer<DType>::ComputeLayerParam() {
	this->m_numOutputRows = this->m_numInputRows;
	this->m_numOutputCols = this->m_numInputCols;
	this->m_outputDepth = this->m_inputDepth;

	// create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
}


template <typename DType>
void DataLayer<DType>::ComputeLayer() {}


template class DataLayer<float>;
template class DataLayer<FixedPoint>;

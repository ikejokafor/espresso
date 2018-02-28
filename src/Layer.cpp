#include "Layer.hpp"
using namespace std;


template <typename DType>
Layer<DType>::Layer   (  
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
                ) {
    m_layerName         = layerName;      
    m_topLayerNames     = topLayerNames;
    m_bottomLayerNames  = bottomLayerNames; 
    m_layerType         = layerType;
    m_numInputRows      = numInputRows;
    m_numInputCols      = numInputCols;
    m_inputDepth        = inputDepth;
    m_outputDepth       = outputDepth;
    m_numKernelRows     = numKernelRows;
    m_numKernelCols     = numKernelCols;
    m_stride            = stride;
    m_padding           = padding;
    m_globalPooling     = globalPooling;
    m_filterData        = filterData;
    m_biasData          = biasData;
    m_group             = group;
    m_localSize         = localSize; 
    m_alpha             = alpha;     
    m_beta              = beta;
    m_length            = length;         
    m_numFracbits       = numFracbits;
    m_blob.data			= NULL;
    m_blob.numRows		= 1;
    m_blob.numCols		= 1;
    m_blob.depth		= 1;

}


template <typename DType>
Layer<DType>::~Layer() {
	if(this->m_filterData) {
		free(this->m_filterData);
	}
	if(this->m_biasData) {
		free(this->m_biasData);
	}
}


template <>
Layer<FixedPoint>::~Layer() {

}


template class Layer<float>;
template class Layer<FixedPoint>;

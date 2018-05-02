#include "Layer.hpp"
using namespace std;


Layer::Layer   (  
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
                ) {
    m_precision             = precision;
    m_layerName             = layerName;      
    m_topLayerNames         = topLayerNames;
    m_bottomLayerNames      = bottomLayerNames; 
    m_layerType             = layerType;
    m_numInputRows          = numInputRows;
    m_numInputCols          = numInputCols;
    m_inputDepth            = inputDepth;
    m_outputDepth           = outputDepth;
    m_dinFxPtLength         = dinFxPtLength;         
    m_dinNumFracBits        = dinNumFracBits;
    m_whtFxPtLength         = whtFxPtLength;         
    m_whtNumFracBits        = whtNumFracBits; 
    m_doutFxPtLength        = doutFxPtLength; 
    m_doutNumFracBits       = doutNumFracBits;      
    m_numKernelRows         = numKernelRows;
    m_numKernelCols         = numKernelCols;
    m_stride                = stride;
    m_padding               = padding;
    m_globalPooling         = globalPooling;
    m_flFilterData          = flFilterData; 
    m_flBiasData            = flBiasData;   
    m_fxFilterData          = fxFilterData; 
    m_fxBiasData            = fxBiasData;
    m_numFilterValues       = numFilterValues;    
    m_group                 = group;
    m_localSize             = localSize; 
    m_alpha                 = alpha;     
    m_beta                  = beta;
    m_blob.flData	        = NULL;
    m_blob.fxData	        = NULL;
    m_blob.numRows		    = 1;
    m_blob.numCols		    = 1;
    m_blob.depth		    = 1;
    m_blob.blobSize         = 1;  
}


Layer::~Layer() {

}

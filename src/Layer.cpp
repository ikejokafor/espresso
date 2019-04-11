#include "Layer.hpp"
using namespace std;
using namespace espresso;


Layer::Layer   (  
						layerInfo_t layerInfo
                ) {
    m_precision					= layerInfo.precision;
    m_layerName					= layerInfo.layerName;      
    m_topLayerNames				= layerInfo.topLayerNames;
    m_bottomLayerNames			= layerInfo.bottomLayerNames; 
    m_layerType					= layerInfo.layerType;
    m_numInputRows				= layerInfo.numInputRows;
    m_numInputCols				= layerInfo.numInputCols;
    m_inputDepth				= layerInfo.inputDepth;
    m_outputDepth				= layerInfo.outputDepth;
    m_dinFxPtLength				= layerInfo.dinFxPtLength;         
    m_dinNumFracBits			= layerInfo.dinNumFracBits;
    m_whtFxPtLength				= layerInfo.whtFxPtLength;         
    m_whtNumFracBits			= layerInfo.whtNumFracBits; 
    m_doutFxPtLength			= layerInfo.doutFxPtLength; 
    m_doutNumFracBits			= layerInfo.doutNumFracBits;              
	m_biasFxPtLength			= layerInfo.biasFxPtLength;
	m_biasNumFracBits			= layerInfo.biasNumFracBits;
	m_leakyFxPtLength			= layerInfo.leakyFxPtLength;
	m_leakyNumFracBits			= layerInfo.leakyNumFracBits;  
    m_numKernelRows				= layerInfo.numKernelRows;
    m_numKernelCols				= layerInfo.numKernelCols;
    m_stride					= layerInfo.stride;
    m_padding					= layerInfo.padding;
    m_globalPooling				= layerInfo.globalPooling;
	m_numFilterValues			= layerInfo.numFilterValues;
	m_darknet_n_param   		= layerInfo.darknet_n_param;
	m_darknet_classes_param		= layerInfo.darknet_classes_param;
	m_darknet_outputs_param		= layerInfo.darknet_outputs_param;
	if(layerInfo.flFilterData) {
		m_flFilterData = (float*)malloc(m_numFilterValues * sizeof(float));
		memcpy(m_flFilterData, layerInfo.flFilterData, m_numFilterValues * sizeof(float));
	}
	if(layerInfo.flBiasData) {
		m_flBiasData = (float*)malloc(m_outputDepth * sizeof(float));
		memcpy(m_flBiasData, layerInfo.flBiasData, m_outputDepth * sizeof(float));
	}    
	if(layerInfo.fxFilterData) {
		m_fxFilterData = (fixedPoint_t*)malloc(m_numFilterValues * sizeof(fixedPoint_t));
		memcpy(m_fxFilterData, layerInfo.fxFilterData, m_numFilterValues * sizeof(fixedPoint_t));
	}
	if(layerInfo.fxBiasData) {
		m_fxBiasData = (fixedPoint_t*)malloc(m_outputDepth * sizeof(fixedPoint_t));
		memcpy(m_fxBiasData, layerInfo.fxBiasData, m_outputDepth * sizeof(fixedPoint_t));
	}
    m_group						= layerInfo.group;
    m_localSize					= layerInfo.localSize; 
    m_alpha						= layerInfo.alpha;
	m_flBeta					= layerInfo.flBeta;
	m_fxBeta					= layerInfo.fxBeta;
	m_activation				= layerInfo.activation;
    m_darknetNormScaleBias		= layerInfo.darknetNormScaleBias;
	m_darknetAct				= layerInfo.darknetAct;
    m_blob.flData	        = NULL;
    m_blob.fxData	        = NULL;
    m_blob.numRows		    = 1;
    m_blob.numCols		    = 1;
    m_blob.depth		    = 1;
    m_blob.blobSize         = 1;
}


Layer::~Layer() {

}

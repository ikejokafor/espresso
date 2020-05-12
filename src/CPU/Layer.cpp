#include "Layer.hpp"


espresso::Layer::Layer(layerInfo_obj layerInfo) {
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
	m_numKernels				= layerInfo.numKernels;
	m_kernelDepth				= layerInfo.kernelDepth;
    m_stride					= layerInfo.stride;
    m_padding					= layerInfo.padding;
    m_globalPooling				= layerInfo.globalPooling;
	m_numFilterValues			= layerInfo.numFilterValues;
	m_darknet_n_param   		= layerInfo.darknet_n_param;
	m_darknet_classes_param		= layerInfo.darknet_classes_param;
	m_darknet_outputs_param		= layerInfo.darknet_outputs_param;
	m_backend					= layerInfo.backend;
	m_fpga_upsample				= false;
	m_fpga_activation			= false;
	if(layerInfo.flFilterData)
	{
		m_flFilterData = new float[m_numFilterValues];
		memcpy(m_flFilterData, layerInfo.flFilterData, m_numFilterValues * sizeof(float));
	}
	if(layerInfo.flBiasData)
	{
		m_flBiasData = new float[m_outputDepth];
		memcpy(m_flBiasData, layerInfo.flBiasData, m_outputDepth * sizeof(float));
	}
	if(layerInfo.fxFilterData)
	{
		m_fxFilterData = new fixedPoint_t[m_numFilterValues];
		memcpy(m_fxFilterData, layerInfo.fxFilterData, m_numFilterValues * sizeof(fixedPoint_t));
	}
	if(layerInfo.fxBiasData)
	{
		m_fxBiasData = new fixedPoint_t[m_outputDepth];
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
    m_blob.flData				= NULL;
    m_blob.fxData				= NULL;
    m_blob.numRows				= 1;
    m_blob.numCols				= 1;
    m_blob.depth				= 1;
    m_blob.blobSize				= 1;
	m_backend					= layerInfo.backend;
	m_kernel_group_arr			= layerInfo.kernel_group_arr;
	m_yolo_net					= layerInfo.yolo_net;
	m_net_idx					= layerInfo.net_idx;
	m_fpga_do_res_layer 		= false;
	m_fpga_upsample 			= false;
	m_fpga_activation 			= false;
	m_fpga_do_res_layer 		= false;
	m_fpga_do_kernel1x1		 	= false;
	m_fpga_merged_1x1			= false;
	m_fpga_krnl_1x1_layer		= false;
}


espresso::Layer::~Layer()
{
	if (m_blob.flData)
	{
		free(m_blob.flData);
	}
	if (m_blob.fxData)
	{
		free(m_blob.fxData);
	}
}

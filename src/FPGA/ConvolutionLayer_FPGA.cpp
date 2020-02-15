#include "ConvolutionLayer_FPGA.hpp"


ConvolutionLayer_FPGA::ConvolutionLayer_FPGA(espresso::layerInfo_obj layerInfo) : Layer(layerInfo) { }


ConvolutionLayer_FPGA::~ConvolutionLayer_FPGA() { }


void ConvolutionLayer_FPGA::ComputeLayer() 
{
	if (m_bottomLayers[0]->m_precision == espresso::FLOAT) 
	{
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for (int i = 0; i < blobSize; i++) 
		{
			fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
		}
	}
	ComputeLayer_FxPt();
}


void ConvolutionLayer_FPGA::ComputeLayer_FlPt() { }


void ConvolutionLayer_FPGA::ComputeLayer_FxPt() 
{ 
	int inputBlobDepth	 = m_bottomLayers[0]->m_blob.depth;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
				
    // adapt_net_allocator* allocator = new adapt_net_allocator();
    // for (int i = 0; i < NUM_AWP; i++)
	// {
	// 	allocator->add_awp(new awp(NUM_QUAD_PER_AWE));
    // }
	// for (int i = 0; i < NUM_FAS; i++)
    // {
	// 	allocator->add_fas(new fas(64, 1, 64));
    // }
	// layer_job* job = new layer_job(m_layerName);
    // job->set_allocator(allocator);
	// job->add_map_descriptor(new map_descriptor(numInputBlobCols, numInputBlobRows, inputBlobDepth));
	// 
	// for (int k = 0; k < m_numKernels; k++)
	// 	job->add_kernel(new kernel(m_numKernelCols, m_numKernelRows, m_kernelDepth, m_whtFxPtLength, m_whtNumFracBits));
	// 
    // job->initialize();
}


void ConvolutionLayer_FPGA::ComputeLayerParam() 
{
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;
	m_numOutputRows = (int)((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = (int)((m_numInputCols - m_numKernelCols + 2 * m_padding) / m_stride) + 1;
	m_numKernels = m_outputDepth;
	m_kernelDepth = m_inputDepth / m_group;
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData =  new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}

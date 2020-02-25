#include "ConvolutionLayer_FPGA.hpp"
using namespace std;

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
	cout << "Layer: " << m_layerName << endl << endl << endl;
	Layer_Job layer_job(		    
		m_inputDepth, 
		m_numInputRows, 
		m_numInputCols,
		m_blob.fxData,
		m_numKernels, 
		m_kernelDepth, 
		m_numKernelRows, 
		m_numKernelCols,
		m_fxFilterData,
		m_outputDepth,
		m_numOutputRows,
		m_numOutputCols,
		m_topLayers[0]->m_blob.fxData,
		m_stride,
		m_fpga_upsample,
		m_padding,
		m_fpga_conv_out_fmt0,
		m_fpga_residual,
		m_fpga_activation
	);
	layer_job.createLayerIters();
	layer_job.process();
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

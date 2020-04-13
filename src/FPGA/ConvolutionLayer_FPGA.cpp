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
	if (m_numKernelRows == 1)
	{
		cout << m_layerName << " Merged" << endl;
	}
	else
	{
		Layer_Job* m_layer_job = new Layer_Job(
			m_layerName,
			m_inputDepth,
			m_numInputRows,
			m_numInputCols,
			m_bottomLayers[0]->m_blob.fxData,
			m_numKernels,
			m_kernelDepth,
			m_numKernelRows,
			m_numKernelCols,
			m_fxFilterData,
			m_outputDepth,
			m_numOutputRows,
			m_numOutputCols,
			m_residualMapDepth,
			m_numResidualMapRows,
			m_numResidualMapsCols,
			m_residualMapData,
			m_kernel1x1Data,
			m_fxBiasData,
			m_bias1x1Data,
			m_stride,
			m_fpga_upsample,
			m_padding,
			m_fpga_do_res_layer,
			m_activation,
			m_fpga_do_kernel1x1,
			m_fpga_hndl
		);
		m_layer_job->createLayerIters();
		m_layer_job->process();
	}

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

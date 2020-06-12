#include "ConvolutionLayer_FPGA.hpp"
using namespace std;

ConvolutionLayer_FPGA::ConvolutionLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


ConvolutionLayer_FPGA::~ConvolutionLayer_FPGA() { }


void ConvolutionLayer_FPGA::ComputeLayer()
{
	// if(m_bottomLayers[0]->m_precision == espresso::FLOAT)
	// {
	// 	int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
	// 	fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
	// 	float        *flData = m_bottomLayers[0]->m_blob.flData;
	// 	for (int i = 0; i < blobSize; i++)
	// 	{
	// 		fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
	// 	}
	// }
	ComputeLayer_FlPt();
}


void ConvolutionLayer_FPGA::ComputeLayer_FlPt()
{
	if(m_numKernelRows == 1 && m_fpga_merged)
	{
		cout << m_layerName << " Merged" << endl;
		return;
	}

	Layer_Job* m_layer_job = new Layer_Job(
		m_layerName,
		m_inputDepth,
		m_numInputRows,
		m_numInputCols,
		m_bottomLayers[0]->m_blob.flData,
		m_numKernels,
		m_kernelDepth,
		m_numKernelRows,
		m_numKernelCols,
		m_flFilterData,
		m_outputDepth,
		m_numOutputRows,
		m_numOutputCols,
		m_residualMapDepth,
		m_numResidualMapRows,
		m_numResidualMapsCols,
		m_residualMapData,
		m_num1x1Kernels,
		m_kernel1x1Depth,
		m_kernel1x1Data,
		m_flBiasData,
		m_bias1x1Data,
		m_stride,
		m_fpga_upsample,
		m_padding,
		m_fpga_do_res_layer,
		m_activation,
		m_fpga_do_kernels1x1,
		m_fpga_hndl,
		m_fpga_krnl_1x1_layer,
        m_fpga_do_1x1_res,
        m_fpga_do_res_1x1
	);
	m_layer_job->createLayerIters();
	m_layer_job->process();
	// delete m_layer_job;
}


void ConvolutionLayer_FPGA::ComputeLayer_FxPt()
{

}


void ConvolutionLayer_FPGA::ComputeLayerParam()
{
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;
	m_numOutputRows = (int)((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = (int)((m_numInputCols - m_numKernelCols + 2 * m_padding) / m_stride) + 1;
	m_outputDepth = m_numKernels;
    // FIXME: create FPGA support for this
	m_kernelDepth = m_inputDepth / m_group;
    if(m_group > 1)
    {
        cout << "FIXME - " << __FILE__ << ":" << __LINE__ << endl;
        exit(0);
    }
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData =  new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}

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
		cout << "[ESPRESSO]: " << m_layerName << " Merged" << endl;
		return;
	}
	
	if(m_numKernelRows > 3 || m_stride > 2 || m_padding > 1 || m_dilation != -1 || (m_numKernelRows == 1 && m_stride > 1))
	{
        cout << "Convolutions with (m_numKernelRows > 3 || m_stride > 2 || m_padding > 1 || m_dilation != -1 || (m_numKernelRows == 1 && m_stride > 1) not supported" << endl;
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
        m_fpga_do_res_layer_only,
        m_fpgaAct3x3,
        m_fpgaAct1x1,
		m_fpga_do_kernels1x1,
		m_fpga_hndl,
		m_fpga_krnl_1x1_layer,
        m_fpga_do_1x1_res,
        m_fpga_do_res_1x1
	);
	m_layer_job->createLayerIters();
    m_fpga_elapsed_time = 0.0f;
    m_fpga_memPower = 0.0f;
	m_fpga_avgIterTime = 0.0f;
    m_peakBW = 0.0f;
	// m_layer_job->process(m_fpga_elapsed_time, m_fpga_avgIterTime, m_fpga_memPower, m_avg_QUAD_time0, m_avg_FAS_time0, m_avg_QUAD_time1, m_avg_FAS_time1);
    m_layer_job->process(m_topLayers[0]->m_blob.flData);

    // FIXME: delete m_layer_job;
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

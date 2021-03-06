#include "ResidualLayer_FPGA.hpp"
using namespace std;


ResidualLayer_FPGA::ResidualLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


ResidualLayer_FPGA::~ResidualLayer_FPGA() { delete m_layer_job; }


void ResidualLayer_FPGA::ComputeLayer()
{
	// if (m_bottomLayers[0]->m_precision == espresso::FLOAT)
	// {
	// 	int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
	// 	fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
	// 	float        *flData = m_bottomLayers[0]->m_blob.flData;
	// 	for (int i = 0; i < blobSize; i++)
	// 	{
	// 		fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
	// 	}
	// }
	ComputeLayer_FxPt();
}


void ResidualLayer_FPGA::ComputeLayer_FlPt() { }


void ResidualLayer_FPGA::ComputeLayer_FxPt()
{
    if(m_fpga_merged)
	{
		cout << "[ESPRESSO]: " << m_layerName << " Merged" << endl;
        cout << endl << endl;
		return;
	}
    
    
    // FILE *fd = fopen("./residualMaps.txt", "w");
    // for(int d = 0; d < m_residualMapDepth; d++)
    // {
    //     for(int r = 0; r < m_numResidualMapRows; r++)
    //     {
    //         for(int c = 0; c < m_numResidualMapsCols; c++)
    //         {
    //             int idx = index3D(m_numResidualMapRows, m_numResidualMapsCols, d, r, c);
    //             fprintf(fd, "%f ", m_residualMapData[idx]);
    //         }
    //         fprintf(fd, "\n");
    //     }
    //     fprintf(fd, "\n\n\n");
    // }
    // fclose(fd);
    m_fpga_elapsed_time = 0.0f;
    m_fpga_memPower = 0.0f;
	m_fpga_avgIterTime = 0.0f;
    m_peakBW = 0.0f;
	m_layer_job->process(m_fpga_elapsed_time, m_fpga_avgIterTime, m_fpga_memPower, m_avg_QUAD_time0, m_avg_FAS_time0, m_avg_QUAD_time1, m_avg_FAS_time1);
	// m_layer_job->process(m_topLayers[0]->m_blob.flData);
    // m_layer_job->process(m_fpga_elapsed_time, m_avg_QUAD_time0, m_avg_FAS_time0);
}



void ResidualLayer_FPGA::ComputeLayerParam()
{
	// input size
	m_inputDepth = m_bottomLayers.size() * m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData = new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}


void ResidualLayer_FPGA::makeLayerJob()
{
    m_layer_job = new Layer_Job(
		m_layerName,
        espresso::to_string(m_layerType),
        m_group,
		m_inputDepth,
		m_numInputRows,
		m_numInputCols,
		m_bottomLayers[0]->m_blob.flData,
		m_numKernels,
		m_kernelDepth,
		m_numKernelRows,
		m_numKernelCols,
		m_flFilterData,
        m_fpga_outDepth,
        m_fpga_numOutRows,
        m_fpga_numOutCols,
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
        m_fpga_do_res_1x1,
        m_first,
        m_last
	);    
}

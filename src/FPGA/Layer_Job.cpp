#include "Layer_Job.hpp"
using namespace std;


Layer_Job::Layer_Job(
	string layerName,
	int inputMapDepth, 
	int numInputMapRows, 
	int numInputMapCols,
	fixedPoint_t* inputMapData,
	int numKernels, 
	int kernelDepth, 
	int numKernelRows, 
	int numKernelCols,
	fixedPoint_t* kernelData,
	int outputMapDepth,
	int numOutputMapRows,
	int numOutputMapCols,
	fixedPoint_t* outputMapData,
	int stride,
	bool upsample,
	int padding,
	bool residual,
	bool activation,
	bool kernel_1x1,
	FPGA_hndl* fpga_hndl,
	int fxPtLength, 
	int numFracBits
) {
	m_layerName				= layerName				;
    m_inputMapDepth         = inputMapDepth			;  
    m_numInputMapRows       = numInputMapRows		;
    m_numInputMapCols       = numInputMapCols		;
	m_outputMapDepth		= outputMapDepth		;
	m_numOutputMapRows		= numOutputMapRows		;
	m_numOutputMapCols		= numOutputMapCols		;	
    m_numKernels            = numKernels            ;
    m_kernelDepth           = kernelDepth           ;
    m_numKernelRows         = numKernelRows         ;
    m_numKernelCols         = numKernelCols         ;
	m_fxPtLength			= fxPtLength			;
	m_numFracBits			= numFracBits			;
	m_stride				= stride				;
	m_upsample				= upsample				;
	m_padding				= padding				;
	m_residual				= residual				;
	m_activation			= activation			;
	m_kernel_1x1			= kernel_1x1			;
	m_residual				= residual				;
	m_inputMaps 			= new InputMaps(inputMapDepth, numInputMapRows, numInputMapCols, inputMapData);
	m_kernels 				= new Kernels(numKernels, kernelDepth, numKernelRows, numKernelCols, kernelData);
	m_outputMaps			= new OutputMaps(outputMapDepth, numOutputMapRows, numOutputMapCols, outputMapData);
#ifdef SYSTEMC
	m_sysc_fpga_hndl		= reinterpret_cast<SYSC_FPGA_hndl*>(fpga_hndl);
#else

#endif
}


Layer_Job::~Layer_Job()
{
	for (int i = 0; i < m_lay_it_arr.size(); i++)
	{
		for(int j = 0; j < m_lay_it_arr[j].size(); j++)
		{
			// delete m_lay_it_arr[i][j];
		}
	}
}


void Layer_Job::createLayerIters()
{
	m_num_Depth_iter = ceil((double)m_inputMapDepth / (double)QUAD_DPTH_SIMD);
	m_num_krnl_iter = ceil((double)m_numKernels / (double)QUAD_MAX_KERNELS);

	
	int remNumKrnl = m_numKernels;
	int numKrnl;
	m_lay_it_arr.resize(m_numKernels);
	for (int i = 0, krnlBgn = 0; i < m_num_krnl_iter; i++, krnlBgn += numKrnl)
	{
		numKrnl = min(remNumKrnl, QUAD_MAX_KERNELS);
		int remDepth = m_inputMapDepth;
		int depth;
		for (int j = 0, depthBgn = 0; j < m_num_Depth_iter; j++, depthBgn += depth)
		{
			depth = min(QUAD_DPTH_SIMD, remDepth);
			InputMaps* inputMaps = m_inputMaps->GetVolume(depthBgn, depth);
			Kernels* kernels = m_kernels->GetVolume(krnlBgn, numKrnl, depthBgn, depth);
			m_lay_it_arr[i].push_back(new Layer_Iteration(
				j,
				(j == m_num_Depth_iter) ? true : false,
				inputMaps,
				kernels,
				m_stride,
				m_upsample,
				m_padding,
				m_kernel_1x1,
				m_residual,
				m_activation
			));
			remDepth -= depth;
		}
		remNumKrnl -= numKrnl;
	}
}


void Layer_Job::process()
{
	// Get configuration
	ofstream fd;
	fd.open(string("./layer_cfg/") + m_layerName + string(".txt"));
	fd << m_layerName << endl;
	for(int k = 0; k < m_lay_it_arr.size(); k++)
	{
		fd << "\tKernel Iteration: " << k << endl;
		for (int d = 0; d < m_lay_it_arr[k].size(); d++)
		{
			m_lay_it_arr[k][d]->m_accelCfg->serialize();
			m_sysc_fpga_hndl->setConfig(m_lay_it_arr[k][d]->m_accelCfg);
			fd << "\t\tDepth Iteration: " << d << endl;
			int numAWP = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr.size();
			auto FAS_cfg_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr;
			for (int a = 0; a < numAWP; a++)
			{
				int numQUADs = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr.size();
				auto QUAD_en_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
				auto QUAD_cfg_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;	
				for (int q = 0; q < numQUADs; q++)
				{
					fd << "\t\t\tQUAD_id: "                 << QUAD_cfg_arr[q]->m_QUAD_id                 << endl;
					if (QUAD_en_arr[q])
					{
						fd << "\t\t\t\tFAS_id: "					<< QUAD_cfg_arr[q]->m_FAS_id                  	<< endl;
						fd << "\t\t\t\tAWP_id: "					<< QUAD_cfg_arr[q]->m_AWP_id                  	<< endl;
						fd << "\t\t\t\tresidual	"					<< FAS_cfg_arr[0]->m_res_layer					<< endl;
						fd << "\t\t\t\tfirst_iter: "			    << FAS_cfg_arr[0]->m_first_iter					<< endl;
						fd << "\t\t\t\tlast_iter: "					<< FAS_cfg_arr[0]->m_last_iter				   	<< endl;
						fd << "\t\t\t\tkernel_1x1"					<< FAS_cfg_arr[0]->m_kernel_1x1				    << endl;
						fd << "\t\t\t\tstride: "					<< QUAD_cfg_arr[q]->m_stride                  	<< endl;
						fd << "\t\t\t\tnum_expd_input_rows: "		<< QUAD_cfg_arr[q]->m_num_expd_input_rows     	<< endl;
						fd << "\t\t\t\tnum_expd_input_cols: "		<< QUAD_cfg_arr[q]->m_num_expd_input_cols     	<< endl;
						fd << "\t\t\t\tcrpd_input_row_start: "		<< QUAD_cfg_arr[q]->m_crpd_input_row_start		<< endl;
						fd << "\t\t\t\tcrpd_input_col_start: "		<< QUAD_cfg_arr[q]->m_crpd_input_col_start    	<< endl;
						fd << "\t\t\t\tcrpd_input_row_end: "		<< QUAD_cfg_arr[q]->m_crpd_input_row_end      	<< endl;
						fd << "\t\t\t\tcrpd_input_col_end: "		<< QUAD_cfg_arr[q]->m_crpd_input_col_end      	<< endl;
						fd << "\t\t\t\tnum_output_rows: "			<< QUAD_cfg_arr[q]->m_num_output_rows         	<< endl;
						fd << "\t\t\t\tnum_output_col: "			<< QUAD_cfg_arr[q]->m_num_output_col          	<< endl;
						fd << "\t\t\t\tnum_kernels: "				<< QUAD_cfg_arr[q]->m_num_kernels             	<< endl;
						fd << "\t\t\t\tmaster_QUAD: "				<< QUAD_cfg_arr[q]->m_master_QUAD             	<< endl;
						fd << "\t\t\t\tcascade: "					<< QUAD_cfg_arr[q]->m_cascade                 	<< endl;
						fd << "\t\t\t\tactivation: "				<< QUAD_cfg_arr[q]->m_activation              	<< endl;
						fd << "\t\t\t\tpadding: "					<< QUAD_cfg_arr[q]->m_padding                 	<< endl;
						fd << "\t\t\t\tupsample: "					<< QUAD_cfg_arr[q]->m_upsample                	<< endl;
					}
					else
					{
						fd << "\t\t\t\tUnused" << endl;
					}
				}
			}
		}
	}
	fd.close();
}

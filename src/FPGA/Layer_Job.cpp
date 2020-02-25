#include "Layer_Job.hpp"
using namespace std;


Layer_Job::Layer_Job(
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
	bool conv_out_fmt0,
	bool residual,
	bool activation,
	int fxPtLength, 
	int numFracBits
) {
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
	m_conv_out_fmt0			= conv_out_fmt0			;
	m_residual				= residual				;
	m_activation			= activation			;
	m_residual				= residual				;
	m_inputMaps 			= new InputMaps(inputMapDepth, numInputMapRows, numInputMapCols, inputMapData);
	m_kernels 				= new Kernels(numKernels, kernelDepth, numKernelRows, numKernelCols, kernelData);
	m_outputMaps			= new OutputMaps(outputMapDepth, numOutputMapRows, numOutputMapCols, outputMapData);
}


Layer_Job::~Layer_Job()
{
	for (int i = 0; i < m_lay_it_arr.size(); i++)
	{
		delete m_lay_it_arr[i];
	}
}


void Layer_Job::createLayerIters()
{
	int num_Depth_iter = ceil((double)m_inputMapDepth / (double)QUAD_DPTH_SIMD);
	int num_krnl_iter = ceil((double)m_numKernels / (double)QUAD_MAX_KERNELS);

	
	int remNumKrnl = m_numKernels;
	int numKrnl;
	cout << num_krnl_iter <<  " Kernel iterations" << endl << endl << endl;
	cout << num_Depth_iter <<  " Depth iterations" << endl << endl << endl;
	for (int i = 0, krnlBgn = 0; i < num_krnl_iter; i++, krnlBgn += numKrnl)
	{
		cout << "Layer Kernel Iteration: " << i << endl << endl << endl;
		numKrnl = min(remNumKrnl, QUAD_MAX_KERNELS);
		int remDepth = m_inputMapDepth;
		int depth;
		int iter = 0;
		for (int j = 0, depthBgn = 0; j < num_Depth_iter; j++, depthBgn += depth)
		{
			depth = min(QUAD_DPTH_SIMD, remDepth);
			InputMaps* inputMaps = m_inputMaps->GetVolume(depthBgn, depth);
			Kernels* kernels = m_kernels->GetVolume(krnlBgn, numKrnl, depthBgn, depth);
			cout << "Layer Depth Iteration: " << iter << endl << endl << endl;
			m_lay_it_arr.push_back(new Layer_Iteration(
				inputMaps,
				kernels,
				m_stride,
				m_upsample,
				m_padding,
				m_conv_out_fmt0,
				m_residual,
				m_activation
			));
			iter++;
			remDepth -= depth;
		}
		remNumKrnl -= numKrnl;
	}
}


void Layer_Job::process()
{
    
}

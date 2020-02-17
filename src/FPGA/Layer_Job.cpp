#include "Layer_Job.hpp"
using namespace std;


Layer_Job::Layer_Job(
	int inputMapDepth, 
	int numInputMapRows, 
	int numInputMapCols, 
	int numKernels, 
	int kernelDepth, 
	int numKernelRows, 
	int numKernelCols,
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
	m_inputMaps 			= new InputMaps(m_inputMapDepth, m_numInputMapRows, m_numInputMapCols);
	m_kernels 				= new Kernels(numKernels, kernelDepth, numKernelRows, numKernelCols);
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
	for (int i = 0, krnl_start = 0; i < num_krnl_iter; i++, krnl_start += numKrnl)
	{
		cout << "Layer Kernel Iteration: " << i << endl << endl << endl;
		numKrnl = min(remNumKrnl, QUAD_MAX_KERNELS);
		int remDepth = m_inputMapDepth;
		int depth;
		int iter = 0;
		for (int j = 0, depth_start = 0; j < num_Depth_iter; j++, depth_start += depth)
		{
			depth = min(QUAD_DPTH_SIMD, remDepth);
			InputMaps* inputMaps = m_inputMaps->GetVolume(depth_start, depth);
			Kernels* kernels = m_kernels->GetVolume(krnl_start, numKrnl, depth_start, depth);
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

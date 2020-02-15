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
	int fxPtLength,
	int numFracBits,
	bool upsample,
	int padding
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
	m_upsample				= upsample				;
	m_padding				= padding				;	
	m_inputMaps 			= new InputMaps(m_inputMapDepth, m_numInputMapRows, m_numInputMapCols);
	m_kernels 				= new Kernels(numKernels, kernelDepth, numKernelRows, numKernelCols);
}


Layer_Job::~Layer_Job()
{
    
}


void Layer_Job::createLayerIters()
{
	int num_inMapDepth_iter = ceil(double(m_inputMapDepth) / double(QUAD_INMAP_DPTH_SIMD));
	int num_krnlDepth_iter = ceil(double(m_kernelDepth) / double(QUAD_KRNL_DPTH_SIMD));
	int num_krnl_iter = ceil(double(m_numKernels) / double(QUAD_MAX_KERNELS));
	for (int i = 0; i < num_inMapDepth_iter; i += QUAD_INMAP_DPTH_SIMD)
	{
		InputMaps* inputMaps = m_inputMaps->GetVolume(i, QUAD_INMAP_DPTH_SIMD);
		for (int j = 0; j < num_krnlDepth_iter; j += QUAD_MAX_KERNELS)
		{
			for (int k = 0; k < num_krnl_iter; k += QUAD_KRNL_DPTH_SIMD)
			{
				Kernels* Kernels = m_kernels->GetVolume(j, QUAD_MAX_KERNELS, k, QUAD_KRNL_DPTH_SIMD);
				m_lay_it_arr.push_back(Layer_Iteration(
					inputMaps,
					Kernels,
					upsample,
					padding
				));
			}
		}
	}
}


void Layer_Job::process()
{
    
}

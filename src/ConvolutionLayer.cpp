#include "ConvolutionLayer.hpp"
using namespace std;
using namespace espresso;


ConvolutionLayer::ConvolutionLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


ConvolutionLayer::~ConvolutionLayer() {
	if(m_blob.flData) {
		free(m_blob.flData);
	}
	if(m_blob.fxData) {
		free(m_blob.fxData);
	}
}


void ConvolutionLayer::ComputeLayerParam() {
	// input size
	m_inputDepth	= m_bottomLayers[0]->m_outputDepth;
	m_numInputRows  = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols  = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_numOutputRows = (int)((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = (int)((m_numInputCols - m_numKernelCols + 2 * m_padding) / m_stride) + 1;
	m_numKernels	= m_outputDepth;
	m_kernelDepth   = m_inputDepth / m_group;

	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}



void ConvolutionLayer::ComputeLayer() {
    if(m_precision == FLOAT) {
	    ComputeLayer_FlPt();
    } else if(m_precision == FIXED) {
	    ComputeLayer_FxPt();
    }    
}


void ConvolutionLayer::ComputeLayer_FlPt() {
    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }
        
    // get input
    float *datain = m_bottomLayers[0]->m_blob.flData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;
        
    // output
    float *dataout = m_topLayers[0]->m_blob.flData;
    float *filters = m_flFilterData;
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	vector<thread> threads(nthreads);
        
        
	for (int t = 0; t < nthreads; t++) {
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t) {
			for (int m = bi_m; m < ei_m; m++) {
				for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
					for(int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
						index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = m_flBiasData[m];
						for (int k = 0; k < m_kernelDepth; k++) {
							for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
								for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
									if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding                        
										index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) += (
											index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, i, j)
											* index4D(m_numKernels, m_kernelDepth, m_numKernelRows, m_numKernelCols, filters, m, k, kr, kc)
										);
									}
								}
							}
						}
					}
				}
			}
		}, t * m_numKernels / nthreads, (t + 1) == nthreads ? m_numKernels : (t + 1) * m_numKernels / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });

	// need this code for group parameter and iterate over groups while changing these pointers
	// dataout += ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
	// datain  += ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols); 
	// filters += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);
	
	if(m_darknetAct) {
		if(m_activation == LEAKY) {
			LeakyActivation_FlPt();
		}
	}
}


void ConvolutionLayer::LeakyActivation_FlPt() {
    // output
    float *dataout = m_topLayers[0]->m_blob.flData;
	int numValues = m_outputDepth * m_numOutputRows * m_numOutputCols;
	
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	vector<thread> threads(nthreads);

	    
	for (int t = 0; t < nthreads; t++) {
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t) {       
			for (int m = bi_m; m < ei_m; m++) {
				dataout[m] = (dataout[m] < 0.0f) ? dataout[m] * 0.1f : dataout[m];
			}
		}, t * numValues / nthreads, (t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}


void ConvolutionLayer::ComputeLayer_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinNumFracBits, flData[i]);
        }
    }
        
    if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength || m_bottomLayers[0]->m_doutNumFracBits != m_dinNumFracBits) {
        fixedPoint::SetParam(   m_bottomLayers[0]->m_doutFxPtLength, 
                                m_bottomLayers[0]->m_doutNumFracBits, 
                                m_dinFxPtLength, 
                                m_dinNumFracBits, 
                                m_bottomLayers[0]->m_blob.fxData,
                                m_bottomLayers[0]->m_blob.blobSize
                            );
    }
        
    // get input
    fixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;
        
    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
        
    //filter data
    fixedPoint_t *filters = (fixedPoint_t*)malloc(sizeof(fixedPoint_t) * m_numFilterValues);
    memcpy(filters, m_fxFilterData, (sizeof(fixedPoint_t) * m_numFilterValues));
    fixedPoint::SetParam(   ESPRO_DEF_FXPT_LEN,
                            ESPRO_DEF_NUM_FRAC_BITS,
                            m_whtFxPtLength,
                            m_whtNumFracBits,
                            filters,
                            m_numFilterValues
                        );
	fixedPoint_t *filters_ptr = filters;
        
    int resFxPtLength  = m_dinFxPtLength + m_whtFxPtLength;
    int resNumFracBits = m_dinNumFracBits + m_whtNumFracBits;
        
    fixedPoint_t *bias = (fixedPoint_t*)malloc(sizeof(fixedPoint_t) * m_numKernels);
    memcpy(bias, m_fxBiasData, (sizeof(fixedPoint_t) * m_numKernels));
    fixedPoint::SetParam(   ESPRO_DEF_FXPT_LEN,
                            ESPRO_DEF_NUM_FRAC_BITS,
                            resFxPtLength,
                            resNumFracBits,
                            bias,
                            m_numKernels
                        );
        
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	vector<thread> threads(nthreads);
        		
	for (int t = 0; t < nthreads; t++) {
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t) {
			for (int m = bi_m; m < ei_m; m++) {
				for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride) {
					for(int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride) {
						index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = bias[m];
						for (int k = 0; k < m_kernelDepth; k++) {
							for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++) {
								for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++) {
									if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) {	// in valid region, assuming zero padding                        
										index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) += (
											index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, k, i, j)
											* index4D(m_numKernels, m_kernelDepth, m_numKernelRows, m_numKernelCols, filters_ptr, m, k, kr, kc)
										);
									}
								}
							}
						}
					}
				}
			}
		}, t * m_numKernels / nthreads, (t + 1) == nthreads ? m_numKernels : (t + 1) * m_numKernels / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
        
	// need this code for group parameter and iterate over groups while changing these pointers
	// dataout += ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
	// datain  += ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols); 
	// filters += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);
		
		
    fixedPoint::SetParam(   resFxPtLength, 
                            resNumFracBits, 
                            m_doutFxPtLength, 
                            m_doutNumFracBits, 
                            m_topLayers[0]->m_blob.fxData,
                            m_topLayers[0]->m_blob.blobSize
                        );
      
	if(m_darknetAct) {
		if(m_activation == LEAKY) {
			LeakyActivation_FxPt();
		}
	}
	
    free(filters);
    free(bias);
}


void ConvolutionLayer::LeakyActivation_FxPt() {
    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
	int numValues = m_outputDepth * m_numOutputRows * m_numOutputCols;

	
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	vector<thread> threads(nthreads);
	fixedPoint_t leakyScaleValue = fixedPoint::create(m_leakyNumFracBits, 0.1f);
	fixedPoint_t one = fixedPoint::create(m_leakyNumFracBits, 1.0f);
	
	int resFxPtLength  = m_dinFxPtLength + m_leakyFxPtLength;
    int resNumFracBits = m_dinNumFracBits + m_leakyNumFracBits;
	
	    
	for (int t = 0; t < nthreads; t++) {
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t) {       
			for (int m = bi_m; m < ei_m; m++) {
				dataout[m] = (dataout[m] < 0.0f) ? dataout[m] * leakyScaleValue : dataout[m] * one;
			}
		}, t * numValues / nthreads, (t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });	
	
	

	fixedPoint::SetParam(   resFxPtLength, 
							resNumFracBits, 
							m_doutFxPtLength, 
							m_doutNumFracBits, 
							m_topLayers[0]->m_blob.fxData,
							m_topLayers[0]->m_blob.blobSize
						);
}

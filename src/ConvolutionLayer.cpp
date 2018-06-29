#include "ConvolutionLayer.hpp"
using namespace std;


ConvolutionLayer::ConvolutionLayer  (
                                        precision_t precision,
                                        string layerName,
                                        vector<string> topLayerNames,
                                        vector<string> bottomLayerNames,
                                        string layerType,
                                        int numInputRows,
                                        int numInputCols,
                                        int inputDepth,
                                        int outputDepth,
                                        int dinFxPtLength,
                                        int dinNumFracBits,
                                        int whtFxPtLength,
                                        int whtNumFracBits,
                                        int doutFxPtLength,
                                        int doutNumFracBits,                                
                                        int numKernelRows,
                                        int numKernelCols,
                                        int stride,
                                        int padding,
                                        bool globalPooling,
                                        float *flFilterData,
                                        float *flBiasData,  
                                        FixedPoint_t *fxFilterData,
                                        FixedPoint_t *fxBiasData, 
                                        int numFilterValues,                                
                                        int group,
                                        int localSize,
                                        float alpha,
                                        float beta
                                    ) : Layer	(	
                                                    precision,
                                                    layerName,
                                                    topLayerNames,
                                                    bottomLayerNames,
                                                    layerType,
                                                    numInputRows,
                                                    numInputCols,
                                                    inputDepth,
                                                    outputDepth,
                                                    dinFxPtLength,
                                                    dinNumFracBits,
                                                    whtFxPtLength,
                                                    whtNumFracBits,
                                                    doutFxPtLength,
                                                    doutNumFracBits,                                            
                                                    numKernelRows,
                                                    numKernelCols,
                                                    stride,
                                                    padding,
                                                    globalPooling,
                                                    flFilterData,
                                                    flBiasData,                                                          
                                                    fxFilterData,                                                        
                                                    fxBiasData, 
                                                    numFilterValues,
                                                    group,
                                                    localSize,
                                                    alpha,
                                                    beta
                                                ) {
}


ConvolutionLayer::~ConvolutionLayer() {
    free(m_blob.flData);
    free(m_blob.fxData);
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
    m_blob.fxData = (FixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(FixedPoint_t));
}



void ConvolutionLayer::ComputeLayer() {

    if(m_precision == FLOAT) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------
        if(m_bottomLayers[0]->m_precision == FIXED) {
            int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
            int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
            FixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
            float        *flData = m_bottomLayers[0]->m_blob.flData;
            for(int i = 0; i < blobSize; i++) {
                flData[i] = FixedPoint::toFloat(dinNumFracBits, fxData[i]);
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
		// dataout += ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
		// datain  += ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols); 
		// filters += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);

        // End Code ---------------------------------------------------------------------------------------------------------------------------------        
        
    } else if(m_precision == FIXED) {
        
        // Begin Code -------------------------------------------------------------------------------------------------------------------------------
        if(m_bottomLayers[0]->m_precision == FLOAT) {
            int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
            FixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
            float        *flData = m_bottomLayers[0]->m_blob.flData;
            for(int i = 0; i < blobSize; i++) {
                fxData[i] = FixedPoint::create(m_dinNumFracBits, flData[i]);
            }
        }
        
        if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength) {
            FixedPoint::SetParam(   m_bottomLayers[0]->m_doutFxPtLength, 
                                    m_bottomLayers[0]->m_doutNumFracBits, 
                                    m_dinFxPtLength, 
                                    m_dinNumFracBits, 
                                    m_bottomLayers[0]->m_blob.fxData,
                                    m_bottomLayers[0]->m_blob.blobSize
                                );
        }
        
        // get input
        FixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
        int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
        int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
        int inputBlobDepth = m_bottomLayers[0]->m_blob.depth;
        
        // output
        FixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
        
        //filter data
        FixedPoint_t *filters = (FixedPoint_t*)malloc(sizeof(FixedPoint_t) * m_numFilterValues);
        memcpy(filters, m_fxFilterData, (sizeof(FixedPoint_t) * m_numFilterValues));
        FixedPoint::SetParam(   ESPRO_DEF_FXPT_LEN,
                                ESPRO_DEF_NUM_FRAC_BITS,
                                m_whtFxPtLength,
                                m_whtNumFracBits,
                                filters,
                                m_numFilterValues
                            );
	    FixedPoint_t *filters_ptr = filters;
        
        int resFxPtLength  = m_dinFxPtLength + m_whtFxPtLength;
        int resNumFracBits = m_dinNumFracBits + m_whtNumFracBits;
        
        FixedPoint_t *bias = (FixedPoint_t*)malloc(sizeof(FixedPoint_t) * m_numKernels);
        memcpy(bias, m_fxBiasData, (sizeof(FixedPoint_t) * m_numKernels));
        FixedPoint::SetParam(   ESPRO_DEF_FXPT_LEN,
                                ESPRO_DEF_NUM_FRAC_BITS,
                                resFxPtLength,
                                resNumFracBits,
                                bias,
                                m_numKernels
                            );
        
        


        for(int g = 0; g < m_group; g++) {
            for (int m = 0; m < (m_numKernels / m_group); m++) {
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
            dataout		+= ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
            datain		+= ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols); 
	        filters_ptr += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);
        }
        
        FixedPoint::SetParam(   resFxPtLength, 
                                resNumFracBits, 
                                m_doutFxPtLength, 
                                m_doutNumFracBits, 
                                m_topLayers[0]->m_blob.fxData,
                                m_topLayers[0]->m_blob.blobSize
                            );
                            
        free(filters);
        free(bias);
        // End Code ---------------------------------------------------------------------------------------------------------------------------------        
        
    }    
}

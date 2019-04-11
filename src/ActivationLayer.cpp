#include "ActivationLayer.hpp"
using namespace std;
using namespace espresso;


ActivationLayer::ActivationLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


ActivationLayer::~ActivationLayer() {}


void ActivationLayer::ComputeLayerParam() {
	// input size
	m_inputDepth   = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;

	// output size
	m_outputDepth = m_inputDepth;
	m_numOutputRows = m_numInputRows;
	m_numOutputCols = m_numInputCols;
    
	// create output blob
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
    m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
    m_blob.flData = (float*)m_topLayers[0]->m_blob.flData;
    m_blob.fxData = (fixedPoint_t*)m_topLayers[0]->m_blob.fxData; 
}


void ActivationLayer::ComputeLayer() {
	if (m_precision == FLOAT) {  
		ComputeLayer_FlPt();
	} else if(m_precision == FIXED) { 
		ComputeLayer_FxPt();
	}
}


void ActivationLayer::ComputeLayer_FlPt() {
	RELUActivation_FlPt();
}


void ActivationLayer::RELUActivation_FlPt() {
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
				for (int x = 0; x < m_numOutputRows; x++) {
					for (int y = 0; y < m_numOutputCols; y++) {
						index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
						    index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0 
					    ) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
					}
				}
			}
		}, t * m_outputDepth / nthreads, (t + 1) == nthreads ? m_outputDepth : (t + 1) * m_outputDepth / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}


void ActivationLayer::ComputeLayer_FxPt() {
	RELUActivation_FxPt();
}


void ActivationLayer::RELUActivation_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
        }
    }
        
    if(m_bottomLayers[0]->m_doutFxPtLength != m_dinFxPtLength){
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
  
    for (int m = 0; m < m_outputDepth; m++) {
        for (int x = 0; x < m_numOutputRows; x++) {
            for(int y = 0; y < m_numOutputCols; y++) {
                index3D(m_outputDepth, m_numOutputRows, m_numOutputCols, dataout, m, x, y) = (
                    index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y) < 0 
                ) ? 0 : index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y);
            }
        }
    }
        
    fixedPoint::SetParam(   m_dinFxPtLength,
                            m_dinNumFracBits, 
                            m_doutFxPtLength, 
                            m_doutNumFracBits, 
                            m_topLayers[0]->m_blob.fxData,
                            m_topLayers[0]->m_blob.blobSize
                        );  
}
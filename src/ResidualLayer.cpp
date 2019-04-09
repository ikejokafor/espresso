#include "ResidualLayer.hpp"
using namespace std;
using namespace espresso;


ResidualLayer::ResidualLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


ResidualLayer::~ResidualLayer() {
	if(m_blob.flData) {
		free(m_blob.flData);
	}
	if(m_blob.fxData) {
		free(m_blob.fxData);
	}
}


void ResidualLayer::ComputeLayerParam() {
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
    m_blob.flData = (float*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(float));
    m_blob.fxData = (fixedPoint_t*)malloc(m_outputDepth * m_numOutputRows * m_numOutputCols * sizeof(fixedPoint_t));
}


void ResidualLayer::ComputeLayer() {
	if (m_precision == FLOAT) {  
		ComputeLayer_FlPt();
	} else if(m_precision == FIXED) { 
		ComputeLayer_FxPt();
	}
}


void ResidualLayer::ComputeLayer_FlPt() {
    if(m_bottomLayers[0]->m_precision == FIXED) {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }
        
	
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
				dataout[m] = m_bottomLayers[0]->m_blob.flData[m] + m_bottomLayers[1]->m_blob.flData[m];
			}
		}, t * numValues / nthreads, (t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}


void ResidualLayer::ComputeLayer_FxPt() {
    if(m_bottomLayers[0]->m_precision == FLOAT) {
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            fxData[i] = fixedPoint::create(m_dinNumFracBits, flData[i]);
        }
    }
        
	
    // output
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
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
				dataout[m] = m_bottomLayers[0]->m_blob.fxData[m] + m_bottomLayers[1]->m_blob.fxData[m];
			}
		}, t * numValues / nthreads, (t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}

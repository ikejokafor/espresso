#include "YOLOLayer.hpp"
using namespace std;
using namespace espresso;


YOLOLayer::YOLOLayer(layerInfo_t layerInfo) : Layer(layerInfo) {}


YOLOLayer::~YOLOLayer() {}


void YOLOLayer::ComputeLayerParam() {
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
}


void YOLOLayer::ComputeLayer() {
	if (m_precision == FLOAT) {  
		ComputeLayer_FlPt();
	} else if(m_precision == FIXED) { 
		ComputeLayer_FxPt();
	}
}


void YOLOLayer::ComputeLayer_FlPt() {
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

        
    // output
    float *dataout = m_topLayers[0]->m_blob.flData;
	
		
	memcpy(dataout, datain, m_bottomLayers[0]->m_blob.blobSize * sizeof(float));
    for(int n = 0; n < m_darknet_n_param; n++){
        int index = entry_index(n * m_numInputCols * m_numInputRows, 0);
        activate_array(dataout + index, 2 * m_numInputCols * m_numInputRows);
        index = entry_index(n * m_numInputCols * m_numInputRows, 4);
        activate_array(dataout + index, (1 + m_darknet_classes_param) * m_numInputCols * m_numInputRows);
    }
}


void YOLOLayer::ComputeLayer_FxPt() {
	return;
}


int YOLOLayer::entry_index(int location, int entry) {
    int n = location / (m_numInputCols * m_numInputRows);
    int loc = location % (m_numInputCols * m_numInputRows);
    return n * m_numInputCols * m_numInputRows * (4 + m_darknet_classes_param + 1) + entry * m_numInputCols * m_numInputRows + loc;
}


void YOLOLayer::activate_array(float *x, const int n) {
    for(int i = 0; i < n; i++) {
	    x[i] = 1.0f / (1.0f + exp(-x[i]));
    }	
}
#include "Network.hpp"
using namespace std;
using namespace espresso;


Network::Network() {}


Network::Network(vector<layerInfo_t> &layerInfo, vector<int> &outputLayers) {
	
	for(int i = 0; i < layerInfo.size(); i++) {
		if (layerInfo[i].layerType == "Input") {
			m_cnn.push_back(new DataLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "Convolution") {
			m_cnn.push_back(new ConvolutionLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "Activation") {
			m_cnn.push_back(new ActivationLayer(layerInfo[i]));
        } else if(layerInfo[i].layerType == "LRN") {
			m_cnn.push_back(new NormLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "Pooling_MAX" || layerInfo[i].layerType == "Pooling_AVG") {
			m_cnn.push_back(new PoolingLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "InnerProduct") {
			m_cnn.push_back(new FullyConnectedLayer(layerInfo[i]));
        } else if(layerInfo[i].layerType == "Concat") {
			m_cnn.push_back(new ConcatLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "Softmax") {
			m_cnn.push_back(new SoftMaxLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "YOLO") {
			m_cnn.push_back(new YOLOLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "UpSample") {
			m_cnn.push_back(new UpSampleLayer(layerInfo[i]));
		} else if(layerInfo[i].layerType == "Residual") {
			m_cnn.push_back(new ResidualLayer(layerInfo[i]));
		} else {
            cout << "[ESPRESSO]: " << "Skipped Loading Layer: " << layerInfo[i].layerName << endl;
        }
	}
  
    for(int i = 0; i < m_cnn.size(); i++) {
        cout << "[ESPRESSO]: Loaded Layer " << i <<  " " << m_cnn[i]->m_layerName << endl;
	}
    
    // look for top layers first and insert 
   	for(int i = 0; i < m_cnn.size(); i++) {	// for every layer
		if (m_cnn[i]->m_layerType != "Input") {
			for (uint32_t j = 0; j < m_cnn[i]->m_topLayerNames.size(); j++) { // for every top layer of the current cnn layer
				for (uint32_t k = 0; k < m_cnn.size(); k++) {	// search for the top layer: where I write my data to
					if (m_cnn[i]->m_topLayerNames[j] == m_cnn[k]->m_layerName) {
						m_cnn[i]->m_topLayers.push_back(m_cnn[k]);
						break;
					}
				}
			}
		}
	}
   

    for(int i = 0; i < m_cnn.size(); i++) {	// for every layer
		if (m_cnn[i]->m_layerType != "Input") {
			for (uint32_t j = 0; j < m_cnn[i]->m_bottomLayerNames.size(); j++) { // for every bottom layer of the current cnn layer
				for (uint32_t k = 0; k < m_cnn.size(); k++) {	// search for the bottom layer: where I get my data from
					if (m_cnn[i]->m_bottomLayerNames[j] == m_cnn[k]->m_layerName) {
						m_cnn[i]->m_bottomLayers.push_back(m_cnn[k]);
						break;
					}
				}
			}           
		}
	}


	// get output layers
	if(outputLayers.size() != 0) {
		for(int i = 0; i < outputLayers.size(); i++) {
			m_outputLayers.push_back(m_cnn[outputLayers[i]]);
		}
	} else {
		stack<Layer*> outputLayers_tmp;
		for(uint32_t i = 0; i < m_cnn.size(); i ++) {
			if(m_cnn[i]->m_layerType != "Activation") {   // Activation is in place      
				outputLayers_tmp.push(m_cnn[i]);
				for(uint32_t j = 0; j < m_cnn.size(); j++) {
					for(uint32_t k = 0; k < m_cnn[j]->m_bottomLayers.size(); k++) {
						if(m_cnn[i]->m_layerName == m_cnn[j]->m_bottomLayers[k]->m_layerName) {
							outputLayers_tmp.pop();
							goto label0;
						}
					}
				}
			}
			label0:
			continue;
		}
		// reverse order
		m_outputLayers.resize(outputLayers.size());
		m_outputLayers[outputLayers.size() - 1] = outputLayers_tmp.top();
		outputLayers_tmp.pop();
		for(uint32_t i = outputLayers.size() - 1; ((int)i) >= 0; i--) {
			m_outputLayers[i] = outputLayers_tmp.top();
			outputLayers_tmp.pop();
		} 
	}
}



Network::~Network() {
	for(int i = 0; i < m_cnn.size(); i++) {
		delete m_cnn[i];
	}
}



void Network::Forward(string start, string end) {
	
    // find start and end
    int startIdx = -1;
    int endIdx = -1;
    if(start == " ") {
        startIdx = 0;
    }
    if(end == " ") {
        endIdx = m_cnn.size() - 1;
    }
    if(startIdx < 0 || endIdx < 0) {
        for(uint32_t i = 0; i < m_cnn.size(); i++){
            if(m_cnn[i]->m_layerName == start) {
                startIdx = i;
            }        
            if(m_cnn[i]->m_layerName == end) {
                endIdx = i;
            }
        }   
    }
    if((startIdx == -1 || endIdx == -1) || (endIdx < startIdx)) {
        cout << "No start layer: " << start << " or end layer: " << end  << " or direction wrong" << endl;
        exit(0);
    }
    
    for (int i = startIdx; i < (endIdx + 1); i++) {
		m_cnn[i]->ComputeLayerParam();
	}
    
    for (int i = startIdx; i < (endIdx + 1); i++) {
        cout << "Layer " << i <<  " " << m_cnn[i]->m_layerName << endl;
		cout << "\t inputDepth:     \t\t"   << m_cnn[i]->m_inputDepth       << endl;
        cout << "\t numInputRows:   \t\t"   << m_cnn[i]->m_numInputRows     << endl;
        cout << "\t numInputCols:   \t\t"   << m_cnn[i]->m_numInputCols     << endl;
		cout << "\t outputDepth:    \t\t"   << m_cnn[i]->m_outputDepth      << endl;
        cout << "\t numOutputRows:  \t\t"   << m_cnn[i]->m_numOutputRows    << endl;
        cout << "\t numOutputCols:  \t\t"   << m_cnn[i]->m_numOutputCols    << endl;       
        if(m_cnn[i]->m_precision == FLOAT) {
            cout << "\t precision:  \t\t\t"   << "FLOAT"                      << endl;
        } else {                                                            
            cout << "\t precision:  \t\t\t"   << "FIXED"                      << endl;
        }
        if(m_cnn[i]->m_layerType == "Convolution" || m_cnn[i]->m_layerType == "InnerProduct") {
            cout << "\t Stride:             \t\t" << m_cnn[i]->m_stride << endl;
            cout << "\t Padding:            \t\t" << m_cnn[i]->m_padding << endl;
            cout << "\t Number of Kernels:  \t\t" << m_cnn[i]->m_numKernels << endl;
            cout << "\t Kernel Depth:       \t\t" << m_cnn[i]->m_kernelDepth << endl;
            cout << "\t Kernel Size:        \t\t" << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
        }
        if(m_cnn[i]->m_layerType == "Pooling_MAX"|| m_cnn[i]->m_layerType == "Pooling_AVG") {
            if(m_cnn[i]->m_layerType == "Pooling_MAX") {
                cout << "\t Pooling Param:  \t\t" << "Max" << endl;
            } else {
                cout << "\t Pooling Param:  \t\t" << "Average" << endl;
            }
            if(m_cnn[i]->m_globalPooling) {
                cout << "\t Global Pooling:  \t\t" << "Yes" << endl;
            } else {
                cout << "\t Global Pooling:  \t\t" << "No" << endl;
            }
            cout << "\t Stride:             \t\t" << m_cnn[i]->m_stride << endl;
            cout << "\t Padding:            \t\t" << m_cnn[i]->m_padding << endl;
            cout << "\t Kernel Size:        \t\t" << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
        }
        cout << "\t dinFxPtLength:   \t\t"   << m_cnn[i]->m_dinFxPtLength     << endl;
		cout << "\t dinNumFracBits:  \t\t"   << m_cnn[i]->m_dinNumFracBits    << endl;
        cout << "\t whtFxPtLength:   \t\t"   << m_cnn[i]->m_whtFxPtLength     << endl;
        cout << "\t whtNumFracBits:  \t\t"   << m_cnn[i]->m_whtNumFracBits    << endl;
        cout << "\t doutFxPtLength:  \t\t"   << m_cnn[i]->m_doutFxPtLength    << endl;
        cout << "\t doutNumFracBits: \t\t"   << m_cnn[i]->m_doutNumFracBits   << endl;        
	}
   
    // Forward Propagation
	for (int i = startIdx; i < (endIdx + 1); i++) {
		m_cnn[i]->ComputeLayer();        
        cout << "Finished Layer" << " " << m_cnn[i]->m_layerName << endl;
	}
}



int Network::ReturnLayerIdx(string name) {
    for(uint32_t i = 0; i < m_cnn.size(); i++) {
        if(m_cnn[i]->m_layerName == name) {
            return i;
        }
    }
    return -1;
}

#include "Network.hpp"
using namespace std;
using namespace espresso;


template <typename DType>
Network<DType>::Network() {}


template <typename DType>
Network<DType>::Network(vector<layerInfo_t<DType>> &layerInfo) {
	
	for (uint32_t i = 0; i < layerInfo.size(); i++) {
		if (layerInfo[i].layerType == "Input") {
			m_cnn.push_back(new DataLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth
			));
		} else if(layerInfo[i].layerType == "Convolution") {
			m_cnn.push_back(new ConvolutionLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
                layerInfo[i].globalPooling,
				layerInfo[i].filterData,
				layerInfo[i].biasData,
                layerInfo[i].group
			));
		} else if(layerInfo[i].layerType == "ReLU") {
			m_cnn.push_back(new RELULayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
                layerInfo[i].globalPooling,
				layerInfo[i].filterData,
				layerInfo[i].biasData
			));
        } else if(layerInfo[i].layerType == "LRN") {
			m_cnn.push_back(new NormLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
                layerInfo[i].globalPooling,
				layerInfo[i].filterData,
				layerInfo[i].biasData,
                layerInfo[i].group,
                layerInfo[i].localSize,
                layerInfo[i].alpha,
                layerInfo[i].beta
			));
		} else if(layerInfo[i].layerType == "Pooling_MAX" || layerInfo[i].layerType == "Pooling_AVE") {
			m_cnn.push_back(new PoolingLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
                layerInfo[i].globalPooling
			));
		} else if(layerInfo[i].layerType == "InnerProduct") {
			m_cnn.push_back(new FullyConnectedLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType,
				layerInfo[i].numInputRows,
				layerInfo[i].numInputCols,
				layerInfo[i].inputDepth,
				layerInfo[i].outputDepth,
				layerInfo[i].numKernelRows,
				layerInfo[i].numKernelCols,
				layerInfo[i].stride,
				layerInfo[i].padding,
                layerInfo[i].globalPooling,
				layerInfo[i].filterData,
				layerInfo[i].biasData
			));
        } else if(layerInfo[i].layerType == "Concat") {
			m_cnn.push_back(new ConcatLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType
			));
		} else if(layerInfo[i].layerType == "Softmax") {
			m_cnn.push_back(new SoftMaxLayer<DType>(
                layerInfo[i].precision,
				layerInfo[i].layerName,
				layerInfo[i].topLayerNames,
				layerInfo[i].bottomLayerNames,
				layerInfo[i].layerType
			));
		} else {
            cout << "[ESPRESSO]: " << "Skipped Loading Layer: " << layerInfo[i].layerName << endl;
        }
	}
  
    for (uint32_t i = 0; i < m_cnn.size(); i++) {
        cout << "[ESPRESSO]: Loaded Layer " << i <<  " " << m_cnn[i]->m_layerName << endl;
	}
    
    // look for top layers first and insert split clones if needed
   	for (uint32_t i = 0; i < m_cnn.size(); i++) {	// for every layer
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
   

    for (uint32_t i = 0; i < m_cnn.size(); i++) {	// for every layer
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
    stack<Layer<DType>*> outputLayers;
    for(uint32_t i = 0; i < m_cnn.size(); i ++) {
        if(m_cnn[i]->m_layerType != "ReLU") {   // ReLU is in place      
            outputLayers.push(m_cnn[i]);
            for(uint32_t j = 0; j < m_cnn.size(); j++) {
                for(uint32_t k = 0; k < m_cnn[j]->m_bottomLayers.size(); k++) {
                    if(m_cnn[i]->m_layerName == m_cnn[j]->m_bottomLayers[k]->m_layerName) {
                        outputLayers.pop();
                        goto end;
                    }
                }
            }
        }
        end:
        continue;
    }
    m_outputLayers.resize(outputLayers.size());
    m_outputLayers[outputLayers.size() - 1] = outputLayers.top();
    outputLayers.pop();
    for(uint32_t i = outputLayers.size() - 1; ((int)i) >= 0; i--) {
        m_outputLayers[i] = outputLayers.top();
        outputLayers.pop();
    } 
    
}


template <typename DType>
Network<DType>::~Network() {
	for (uint32_t i = 0; i < m_cnn.size(); i++) {
		delete m_cnn[i];
	}
}


template <typename DType>
void Network<DType>::Forward(string start, string end) {
	
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
        if(m_cnn[i]->m_layerType == "Convolution" || m_cnn[i]->m_layerType == "InnerProduct") {
            cout << "\t Stride:             \t\t" << m_cnn[i]->m_stride << endl;
            cout << "\t Padding:            \t\t" << m_cnn[i]->m_padding << endl;
            cout << "\t Number of Kernels:  \t\t" << m_cnn[i]->m_numKernels << endl;
            cout << "\t Kernel Depth:       \t\t" << m_cnn[i]->m_kernelDepth << endl;
            cout << "\t Kernel Size:        \t\t" << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
        }
        if(m_cnn[i]->m_layerType == "Pooling_MAX"|| m_cnn[i]->m_layerType == "Pooling_AVE") {
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
	}
   
    // Forward Propagation
	for (int i = startIdx; i < (endIdx + 1); i++) {
		m_cnn[i]->ComputeLayer();
        if(i > 0 && (m_cnn[i]->m_layerType == "Convolution" || m_cnn[i]->m_layerType == "InnerProduct")) {
            FixedPoint::SetParam(   64, 
                                    32, 
                                    32, 
                                    16, 
                                    m_cnn[i]->m_blob.data,
                                    m_cnn[i]->m_blob.numRows * m_cnn[i]->m_blob.numCols * m_cnn[i]->m_blob.depth
                                );
            

            // if(m_cnn[i].m_precision == FIXED && m_cnn[i].m_topLayers[0].m_precision == FIXED
            //     && m_cnn[i].m_fxPtLength != m_cnn[i].m_topLayers[0].m_fxPtLength && m_cnn[i].m_numFracBits != m_cnn[i].m_topLayers[0].m_numFracBits) {
            //     FixedPoint::SetParam(   m_cnn[i]->m_fxPtLength, 
            //                             m_cnn[i]->m_numFracBits, 
            //                             m_cnn[i]->m_topLayers[0].m_fxPtLength, 
            //                             m_cnn[i]->m_topLayers[0].m_numFracBits, 
            //                             m_cnn[i]->m_blob.data,
            //                             m_cnn[i]->m_blob.numRows * m_cnn[i].m_blob.numCols * m_cnn[i].m_blob.depth
            //                         );
        // }
        }
        
        cout << "Finished Layer" << " " << m_cnn[i]->m_layerName << endl;
	}
}


template <typename DType>
int Network<DType>::ReturnLayerIdx(string name) {
    for(uint32_t i = 0; i < m_cnn.size(); i++) {
        if(m_cnn[i]->m_layerName == name) {
            return i;
        }
    }
    return -1;
}


template class Network<float>;
template class Network<FixedPoint_t>;

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include "Layer.hpp"
#include "DataLayer.hpp"
#include "ConvolutionLayer.hpp"
#include "ActivationLayer.hpp"
#include "PoolingLayer.hpp"
#include "ConcatLayer.hpp"
#include "FullyConnectedLayer.hpp"
#include "SoftMaxLayer.hpp"
#include "NormLayer.hpp"
#include "YOLOLayer.hpp"
#include "UpSampleLayer.hpp"
#include "ResidualLayer.hpp"


#include <vector>


class Network {

	public:
        Network();
        Network(std::vector<espresso::layerInfo_t> &layerInfo, std::vector<int> &outputLayers);
        ~Network();
        void Forward(std::string start = " ", std::string end = " ");
        int ReturnLayerIdx(std::string name);
        
		std::vector<Layer*> m_cnn;
        std::vector<Layer*> m_outputLayers;
        
	protected:


	private:

};

#endif
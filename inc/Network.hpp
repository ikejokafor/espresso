#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "Layer.hpp"
#include "DataLayer.hpp"
#include "ConvolutionLayer.hpp"
#include "RELULayer.hpp"
#include "PoolingLayer.hpp"
#include "ConcatLayer.hpp"
#include "FullyConnectedLayer.hpp"
#include "SoftMaxLayer.hpp"
#include "caffeDataParser.hpp"


#include <vector>


class Network {

	public:
		Network(std::vector<layerInfo_t> &layerInfo);
		~Network();
		void Forward(Blob_t inputBlob, Blob_t *outputBlob);
			

		std::vector<Layer*> m_cnn;

	protected:


	private:

};

#endif
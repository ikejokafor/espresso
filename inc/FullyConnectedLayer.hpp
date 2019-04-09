#ifndef __FULLY_CONNECTED_LAYER_HPP__
#define __FULLY_CONNECTED_LAYER_HPP__


#include "Layer.hpp"


class FullyConnectedLayer : public Layer {
	
	public:
        FullyConnectedLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~FullyConnectedLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
	

	protected:


	private:

};

#endif
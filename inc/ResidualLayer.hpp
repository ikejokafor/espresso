#ifndef __RESIDUAL_LAYER_HPP__
#define __RESIDUAL_LAYER_HPP__


#include "Layer.hpp"


class ResidualLayer : public Layer {
	
	public:
        ResidualLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~ResidualLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();


	protected:


	private:

};

#endif
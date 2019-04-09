#ifndef __ACTIVATION_LAYER_HPP__
#define __ACTIVATION_LAYER_HPP__


#include "Layer.hpp"


class ActivationLayer : public Layer {
	
	public:
		ActivationLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t()); 
        ~ActivationLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void RELUActivation_FlPt();
		void ComputeLayer_FxPt();
		void RELUActivation_FxPt();
	

	protected:


	private:

};


#endif

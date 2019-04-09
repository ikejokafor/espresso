#ifndef __SOFTMAX_LAYER_HPP__
#define __SOFTMAX_LAYER_HPP__


#include "Layer.hpp"


class SoftMaxLayer : public Layer {
	
	public:
        SoftMaxLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~SoftMaxLayer();
        void ComputeLayerParam();   
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
	

	protected:


	private:

};

#endif
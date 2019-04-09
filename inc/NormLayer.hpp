#ifndef __NORM_LAYER_HPP__
#define __NORM_LAYER_HPP__


#include "Layer.hpp"


class NormLayer : public Layer {
	
	public:
        NormLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t()); 
        ~NormLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
	

	protected:


	private:

};


#endif

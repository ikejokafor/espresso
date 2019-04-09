#ifndef __UPSAMPLE_LAYER_HPP__
#define __UPSAMPLE_LAYER_HPP__


#include "Layer.hpp"


class UpSampleLayer : public Layer {
	
	public:
        UpSampleLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t()); 
        ~UpSampleLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();


	protected:


	private:

};

#endif
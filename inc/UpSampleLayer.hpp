#pragma once


#include "Layer.hpp"


class UpSampleLayer : public espresso::Layer 
{
	public:
        UpSampleLayer(espresso::layerInfo_obj layerInfo = espresso::layerInfo_obj()); 
        ~UpSampleLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
};

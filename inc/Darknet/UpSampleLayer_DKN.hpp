#pragma once


#include "Layer.hpp"


class UpSampleLayer_DKN : public espresso::Layer 
{	
	public:
        UpSampleLayer_DKN(espresso::layerInfo_obj layerInfo = espresso::layerInfo_obj()); 
        ~UpSampleLayer_DKN();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam(); 
};

#pragma once


#include "Layer.hpp"


class UpSampleLayer_FPGA : public espresso::Layer 
{	
	public:
        UpSampleLayer_FPGA(espresso::layerInfo_obj layerInfo = espresso::layerInfo_obj()); 
        ~UpSampleLayer_FPGA();      
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();  
};

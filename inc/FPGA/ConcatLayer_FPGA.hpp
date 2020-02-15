#pragma once


#include "Layer.hpp"


class ConcatLayer_FPGA : public espresso::Layer 
{
	
	public:
		ConcatLayer_FPGA(espresso::layerInfo_obj layerInfo = espresso::layerInfo_obj());
		~ConcatLayer_FPGA();         
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

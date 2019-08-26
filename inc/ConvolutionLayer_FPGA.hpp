#pragma once


#include "Layer.hpp"


class ConvolutionLayer_FPGA : public espresso::Layer 
{
	public:
		ConvolutionLayer_FPGA(espresso::layerInfo_obj layerInfo = espresso::layerInfo_obj());
        ~ConvolutionLayer_FPGA();
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};


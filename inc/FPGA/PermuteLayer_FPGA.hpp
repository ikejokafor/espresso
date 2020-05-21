#pragma once


#include "Layer.hpp"


class PermuteLayer_FPGA : public espresso::Layer
{
	public:
        PermuteLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~PermuteLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
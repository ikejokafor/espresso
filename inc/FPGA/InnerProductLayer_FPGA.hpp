#pragma once


#include "Layer.hpp"


class InnerProductLayer_FPGA : public espresso::Layer
{
	public:
        InnerProductLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~InnerProductLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

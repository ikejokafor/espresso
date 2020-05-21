#pragma once


#include "Layer.hpp"


class SoftMaxLayer_FPGA : public espresso::Layer
{
	public:
        SoftMaxLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~SoftMaxLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

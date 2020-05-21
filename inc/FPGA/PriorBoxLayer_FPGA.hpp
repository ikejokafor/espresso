#pragma once


#include "Layer.hpp"


class PriorBoxLayer_FPGA : public espresso::Layer
{
	public:
        PriorBoxLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~PriorBoxLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
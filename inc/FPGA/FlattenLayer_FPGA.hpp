#pragma once


#include "Layer.hpp"


class FlattenLayer_FPGA : public espresso::Layer
{
	public:
        FlattenLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~FlattenLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
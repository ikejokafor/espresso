#pragma once


#include "Layer.hpp"


class ResidualLayer_FPGA : public espresso::Layer
{
	public:
        ResidualLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ResidualLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
        void makeLayerJob();
};

#pragma once


#include "Layer.hpp"


class ReshapeLayer_FPGA : public espresso::Layer
{
	public:
        ReshapeLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ReshapeLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

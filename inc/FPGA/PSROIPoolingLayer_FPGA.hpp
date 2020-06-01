#pragma once


#include "Layer.hpp"


class PSROIPoolingLayer_FPGA : public espresso::Layer
{
	public:
        PSROIPoolingLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~PSROIPoolingLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
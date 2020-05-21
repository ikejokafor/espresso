#pragma once


#include "Layer.hpp"


class DetectionOutputLayer_FPGA : public espresso::Layer
{
	public:
        DetectionOutputLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~DetectionOutputLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
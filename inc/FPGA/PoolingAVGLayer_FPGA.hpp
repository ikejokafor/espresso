#pragma once


#include "Layer.hpp"


class PoolingAVGLayer_FPGA : public espresso::Layer
{
	public:
        PoolingAVGLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~PoolingAVGLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
#pragma once


#include "Layer.hpp"


class PoolingMAXLayer_FPGA : public espresso::Layer
{
	public:
        PoolingMAXLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
        ~PoolingMAXLayer_FPGA();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};
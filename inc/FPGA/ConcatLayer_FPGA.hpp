#pragma once


#include "Layer.hpp"


class ConcatLayer_FPGA : public espresso::Layer
{

	public:
		ConcatLayer_FPGA(espresso::layerInfo_obj* layerInfo = nullptr);
		~ConcatLayer_FPGA();
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

#pragma once

#include "Layer.hpp"


class ResidualLayer : public espresso::Layer
{
	public:
        ResidualLayer(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ResidualLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
};

#pragma once


#include "Layer.hpp"


class ResidualLayer_DKN : public espresso::Layer
{
	public:
        ResidualLayer_DKN(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ResidualLayer_DKN();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

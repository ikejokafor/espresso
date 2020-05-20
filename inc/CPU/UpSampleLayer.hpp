#pragma once


#include "Layer.hpp"


class UpSampleLayer : public espresso::Layer
{
	public:
        UpSampleLayer(espresso::layerInfo_obj* layerInfo = nullptr);
        ~UpSampleLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
};

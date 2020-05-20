#pragma once


#include "Layer.hpp"


class ConcatLayer : public espresso::Layer
{
	public:
        ConcatLayer(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ConcatLayer();
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

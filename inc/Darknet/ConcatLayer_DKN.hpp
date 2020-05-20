#pragma once


#include "Layer.hpp"


class ConcatLayer_DKN : public espresso::Layer
{

	public:
		ConcatLayer_DKN(espresso::layerInfo_obj* layerInfo = nullptr);
		~ConcatLayer_DKN();
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

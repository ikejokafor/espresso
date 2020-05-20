#pragma once


#include "Layer.hpp"


class ConvolutionLayer_DKN : public espresso::Layer
{
	public:
		ConvolutionLayer_DKN(espresso::layerInfo_obj* layerInfo = nullptr);
		~ConvolutionLayer_DKN();
		void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

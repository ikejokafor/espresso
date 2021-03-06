#pragma once


#include "Layer.hpp"


class ConvolutionLayer : public espresso::Layer
{
	public:
		ConvolutionLayer(espresso::layerInfo_obj* layerInfo = nullptr);
        ~ConvolutionLayer();
        void ComputeLayer();
		void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
		void LeakyActivation_FlPt();
		void LeakyActivation_FxPt();
};

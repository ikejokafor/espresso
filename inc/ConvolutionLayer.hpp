#ifndef __CONVOLUTION_LAYER_HPP__
#define __CONVOLUTION_LAYER_HPP__


#include "Layer.hpp"


class ConvolutionLayer : public Layer {
	
	public:
        ConvolutionLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~ConvolutionLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void LeakyActivation_FlPt();
		void LeakyActivation_FxPt();
	

	protected:


	private:

};

#endif
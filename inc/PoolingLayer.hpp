#ifndef __POOLING_LAYER_HPP__
#define __POOLING_LAYER_HPP__


#include "Layer.hpp"


class PoolingLayer : public Layer {
	
	public:
        PoolingLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~PoolingLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void MaxGlobalPooling_FlPt();
		void AvgGlobalPooling_FlPt();
		void MaxPooling_FlPt();
		void AvgPooling_FlPt();
		void ComputeLayer_FxPt();
		void MaxGlobalPooling_FxPt();
		void AvgGlobalPooling_FxPt();
		void MaxPooling_FxPt();
		void AvgPooling_FxPt();
	

	protected:


	private:

};

#endif
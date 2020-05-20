#pragma once


#include "Layer.hpp"


class YOLOLayer : public espresso::Layer {

	public:
		YOLOLayer(espresso::layerInfo_obj* layerInfo = nullptr);
        ~YOLOLayer();
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
        void ComputeLayer_FxPt();
		int entry_index(int location, int entry);
		void activate_array(float *x, const int n);
};

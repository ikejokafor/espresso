#pragma once


#include "Layer.hpp"


class DataLayer : public espresso::Layer 
{
	public:
        DataLayer(espresso::layerInfo_obj layerInfo  = espresso::layerInfo_obj());
        ~DataLayer();               
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
		void ComputeLayerParam();
};

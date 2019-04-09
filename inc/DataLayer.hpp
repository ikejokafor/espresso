#ifndef __DATA_LAYER_HPP__
#define __DATA_LAYER_HPP__


#include "Layer.hpp"


class DataLayer : public Layer {
	
	public:
        DataLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~DataLayer();               
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();
	

	protected:


	private:

};

#endif
#ifndef __CONCAT_LAYER_HPP__
#define __CONCAT_LAYER_HPP__


#include "Layer.hpp"


class ConcatLayer : public Layer {
	
	public:
        ConcatLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t());
        ~ConcatLayer();         
        void ComputeLayerParam();
        void ComputeLayer();
	    void ComputeLayer_FlPt();
		void ComputeLayer_FxPt();


	protected:


	private:

};

#endif
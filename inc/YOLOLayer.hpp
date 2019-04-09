#ifndef __YOLO_LAYER_HPP__
#define __YOLO_LAYER_HPP__


#include "Layer.hpp"


class YOLOLayer : public Layer {
	
	public:
		YOLOLayer(espresso::layerInfo_t layerInfo = espresso::layerInfo_t()); 
        ~YOLOLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();
	    void ComputeLayer_FlPt();
        void ComputeLayer_FxPt();
		int entry_index(int location, int entry);
		void activate_array(float *x, const int n);

	protected:


	private:

};


#endif

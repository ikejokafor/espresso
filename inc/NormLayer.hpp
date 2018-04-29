#ifndef __NORM_LAYER_HPP__
#define __NORM_LAYER_HPP__


#include "Layer.hpp"


class NormLayer : public Layer {
	
	public:
        NormLayer	(
                            precision_t precision = FLOAT, 
                            std::string layerName = " ",
                            std::vector<std::string> topLayerNames = std::vector<std::string>(),
                            std::vector<std::string> bottomLayerNames = std::vector<std::string>(),
                            std::string layerType = " ",
                            int numInputRows = 1,
                            int numInputCols = 1,
                            int inputDepth = 1,
                            int outputDepth = 1,
                            int numKernelRows = 1,
                            int numKernelCols = 1,
                            int stride = 1,
                            int padding = 0,
                            bool globalPooling = false,
                            float *flFilterData = NULL,
                            float *flBiasData = NULL,
                            FixedPoint_t *fxFilterData = NULL,
                            FixedPoint_t *fxBiasData = NULL,
                            int group = 1,
                            int localSize = 5,
                            float alpha = 0.0001f,
                            float beta = 0.75f,
                            int dinFxPtLength = 32,
                            int dinNumFracBits = 16,
                            int whtFxPtLength = 32,
                            int whtNumFracBits = 16                                
                    );  
        ~NormLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();


	protected:


	private:

};


#endif

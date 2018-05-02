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
                        int dinFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int dinNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,
                        int whtFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int whtNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,
                        int doutFxPtLength = ESPRO_DEF_FXPT_LEN,
                        int doutNumFracBits = ESPRO_DEF_NUM_FRAC_BITS,                               
                        int numKernelRows = 1,
                        int numKernelCols = 1,
                        int stride = 1,
                        int padding = 0,
                        bool globalPooling = false,
                        float *flFilterData = NULL,
                        float *flBiasData = NULL,
                        FixedPoint_t *fxFilterData = NULL,
                        FixedPoint_t *fxBiasData = NULL,
                        int numFilterValues = 1,
                        int group = 1,
                        int localSize = 5,
                        float alpha = 0.0001f,
                        float beta = 0.75f                      
                    ); 
        ~NormLayer();      
        void ComputeLayerParam();  
        void ComputeLayer();


	protected:


	private:

};


#endif

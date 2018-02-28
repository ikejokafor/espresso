#ifndef __POOLING_LAYER_HPP__
#define __POOLING_LAYER_HPP__


#include "Layer.hpp"


template <typename DType>
class PoolingLayer : public Layer<DType> {
	
	public:
        PoolingLayer	(
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
                            DType *filterData = NULL,
                            DType *biasData = NULL,
                            int group = 1,
                            int localSize = 5,
                            float alpha = 0.0001f,
                            float beta = 0.75f,
                            int length = 16,
                            int numFracbits = 14                            
                        );
        ~PoolingLayer();
        void ComputeLayerParam();
        void ComputeLayer();


	protected:


	private:

};

#endif
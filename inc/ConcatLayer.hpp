#ifndef __CONCAT_LAYER_HPP__
#define __CONCAT_LAYER_HPP__
#include "Layer.hpp"

template <typename DType>
class ConcatLayer : public Layer<DType> {
	
	public:
        ConcatLayer		(
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
                            DType *filterData = NULL,
                            DType *biasData = NULL,
                            int length = 16,
                            int numFracbits = 14
                        );
        ~ConcatLayer();         
        void ComputeLayerParam();
        void ComputeLayer();



	protected:


	private:

};

#endif
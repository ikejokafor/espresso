#ifndef __CONVOLUTION_LAYER_H__
#define __CONVOLUTION_LAYER_H__
#include "Layer.hpp" 

class ConvolutionLayer : public Layer {
	
	public:
		ConvolutionLayer	(
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
								float *filterData = NULL,
								float *biasData = NULL
							);
		void ComputeLayerParam();
		void ComputeLayer(Blob_t inputBlob);
		~ConvolutionLayer();


	protected:


	private:

};

#endif
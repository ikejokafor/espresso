#ifndef __DATA_LAYER_HPP__
#define __DATA_LAYER_HPP__
#include "Layer.hpp"

class DataLayer : public Layer {
	
	public:
		DataLayer	(
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
		void ComputeLayer();
		~DataLayer();


	protected:


	private:

};

#endif
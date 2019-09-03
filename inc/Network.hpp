#pragma once


// Project Includes
#include "Layer.hpp"
#include "DataLayer.hpp"
#include "ConcatLayer.hpp"
#include "ConcatLayer_DKN.hpp"
#include "ConcatLayer_FPGA.hpp"
#include "ConvolutionLayer.hpp"
#include "ConvolutionLayer_DKN.hpp"
#include "ConvolutionLayer_FPGA.hpp"
#include "ResidualLayer.hpp"
#include "ResidualLayer_DKN.hpp"
#include "ResidualLayer_FPGA.hpp"
#include "UpSampleLayer.hpp"
#include "UpSampleLayer_DKN.hpp"
#include "UpSampleLayer_FPGA.hpp"
#include "YOLOLayer.hpp"

namespace espresso 
{
	class Network {

		public:
			Network(std::vector<espresso::layerInfo_obj> layerInfo, std::vector<int> &outputLayers);
			~Network();
			void Forward(std::string start = " ", std::string end = " ");
			int ReturnLayerIdx(std::string name);
			void printLayerStats(int i);
        
			std::vector<espresso::Layer*> m_cnn;
			std::vector<espresso::Layer*> m_outputLayers;
        
		protected:


		private:

	};
}
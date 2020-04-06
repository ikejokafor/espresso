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
	class CNN_Network 
    {
		public:
			CNN_Network(std::vector<espresso::layerInfo_obj> layerInfo, std::vector<int> &outputLayers);
			~CNN_Network();
			
			void GetTopAndBottomLayers();
			void GetOutputLayers(std::vector<int> &outputLayers);
			void getBgnEndLayer(int& startIdx, std::string start, int& endIdx, std::string end);
			void cfgFPGALayer();
			void Forward(std::string start = " ", std::string end = " ");
			void cfgLayers(int startIdx, int endIdx);
			int ReturnLayerIdx(std::string name);
			void printLayerStats(int i);
			void setHardware(FPGA_hndl* fpga_hndl);
        
			std::vector<espresso::Layer*> m_cnn;
			std::vector<espresso::Layer*> m_outputLayers;
			FPGA_hndl* m_fpga_hndl;
		protected:


		private:

	};
}
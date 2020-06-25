#pragma once


// System Includes
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>


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
#include "FlattenLayer_FPGA.hpp"
#include "InnerProductLayer_FPGA.hpp"
#include "PermuteLayer_FPGA.hpp"
#include "PoolingAVGLayer_FPGA.hpp"
#include "PoolingMAXLayer_FPGA.hpp"
#include "PriorBoxLayer_FPGA.hpp"
#include "ReshapeLayer_FPGA.hpp"
#include "SoftMaxLayer_FPGA.hpp"
#include "DetectionOutputLayer_FPGA.hpp"
#include "PSROIPoolingLayer_FPGA.hpp"


namespace espresso
{
	class CNN_Network
    {
		public:
			CNN_Network(std::vector<espresso::layerInfo_obj*>& layerInfoArr, std::vector<int> &outputLayers);
			~CNN_Network();

			void GetTopAndBottomLayers();
			void GetOutputLayers(std::vector<int> &outputLayers);
			void getBgnEndLayer(int& startIdx, std::string start, int& endIdx, std::string end);
            void cfgFPGALayers();
			void cfgFPGALayers(std::string mrgFmt_fn);
			void Forward(std::string start = "", std::string end = "");
			int ReturnLayerIdx(std::string name);
			void printLayerStats(int i);
			void setHardware(FPGA_hndl* fpga_hndl);
            int findLayer(std::string layerName);
            void mergeLayers(int idx, int seqID, std::vector<std::string>& sequence);
            void printMemBWStats();
            void printExecutionStats();
			std::string to_string(layerType_t layerType);


            std::vector<int> seqBgnIdxArr;
			std::vector<espresso::Layer*> m_cnn;
			std::vector<espresso::Layer*> m_outputLayers;
			FPGA_hndl* m_fpga_hndl;
		protected:


		private:

	};
}
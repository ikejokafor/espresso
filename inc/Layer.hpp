#pragma once


// System Includes
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <random>
#include <float.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
#include <functional>


// Project Includes
#include "activation_layer.h"
#include "logistic_layer.h"
#include "l2norm_layer.h"
#include "activations.h"
#include "avgpool_layer.h"
#include "batchnorm_layer.h"
#include "blas.h"
#include "connected_layer.h"
#include "deconvolutional_layer.h"
#include "convolutional_layer.h"
#include "cost_layer.h"
#include "crnn_layer.h"
#include "crop_layer.h"
#include "detection_layer.h"
#include "dropout_layer.h"
#include "gru_layer.h"
#include "list.h"
#include "local_layer.h"
#include "maxpool_layer.h"
#include "normalization_layer.h"
#include "option_list.h"
#include "parser.h"
#include "region_layer.h"
#include "yolo_layer.h"
#include "iseg_layer.h"
#include "reorg_layer.h"
#include "rnn_layer.h"
#include "route_layer.h"
#include "upsample_layer.h"
#include "shortcut_layer.h"
#include "softmax_layer.h"
#include "lstm_layer.h"
#include "util.hpp"
#include "fixedPoint.hpp"
// #ifdef FPGA
#include "Layer_Job.hpp"
// #endif



namespace espresso
{
	// Definitions
	#define HIGH_PERF
	const float THRESH = 0.5f;
	const float HIER_THRESH	= 0.5f;
	const float NMS_THRESH = 0.45f;
	const int YOLO_DEF_FXPT_LEN	= 16;
	const int YOLO_DEF_NUM_FRAC_BITS = 14;
	const int YOLO_MAX_NUM_INT_BITS = (YOLO_DEF_FXPT_LEN - 1);
	const int YOLO_MAX_NUM_FRAC_BITS = (YOLO_DEF_FXPT_LEN - 2);


	// User Defined Types
	typedef struct
	{
		float *flData;
		fixedPoint_t *fxData;
		int blobSize;
		int numRows;
		int numCols;
		int depth;
	}  Blob_t;


	typedef enum
	{
		FLOAT = 0,
		FIXED = 1
	} precision_t;


	typedef enum
	{
		LEAKY   = 0,
		RELU    = 1,
		LINEAR  = 2
	} activation_t;


	typedef enum
	{
		FPGA_BACKEND        = 0,
		DARKNET_BACKEND     = 1,
		ESPRESSO_BACKEND    = 2
	} backend_t;


	typedef enum
	{
        INPUT               = 0,
        CONVOLUTION         = 1,
        POOLING_MAX         = 2,
        POOLING_AVG         = 3,
        PERMUTE             = 4,
        FLATTEN             = 5,
        RESIDUAL            = 6,
        DETECTION_OUTPUT    = 7,
        PRIOR_BOX           = 8,
        RESHAPE             = 9,
        INNERPRODUCT        = 10,
        SOFTMAX             = 11,
        CONCAT              = 12,
        YOLO                = 13,
        UPSAMPLE            = 14,
        PSROIPoolingLayer   = 15
	} layerType_t;


    class layerInfo_obj
	{
		public:
			layerInfo_obj()
			{
				precision = espresso::FLOAT;
				layerType = espresso::INPUT;
				numInputRows = 1;
				numInputCols = 1;
				inputDepth = 1;
				outputDepth = 1;
				dinFxPtLength = YOLO_DEF_FXPT_LEN;
				dinNumFracBits = YOLO_DEF_NUM_FRAC_BITS;
				whtFxPtLength = YOLO_DEF_FXPT_LEN;
				whtNumFracBits = YOLO_DEF_NUM_FRAC_BITS;
				doutFxPtLength = YOLO_DEF_FXPT_LEN;
				doutNumFracBits = YOLO_DEF_NUM_FRAC_BITS;
				biasFxPtLength = YOLO_DEF_FXPT_LEN;
				biasNumFracBits = YOLO_DEF_NUM_FRAC_BITS;
				leakyFxPtLength = YOLO_DEF_FXPT_LEN;
				leakyNumFracBits = YOLO_DEF_NUM_FRAC_BITS;
				numKernels = 1;
				kernelDepth = 1;
				numKernelRows = 1;
				numKernelCols = 1;
				stride = 1;
				padding = 0;
				globalPooling = false;
				numFilterValues = 1;
				flFilterData = NULL;
				flBiasData = NULL;
				fxFilterData = NULL;
				fxBiasData = NULL;
				group = 1;
				localSize = 5;
				alpha = 0.0001f;
				flBeta = 0.75f;
				fxBeta = 0.000001f;
				activation = espresso::RELU;
				darknetNormScaleBias = false;
				darknetAct = false;
				darknet_n_param = 0;
				darknet_classes_param = 0;
				darknet_outputs_param = 0;
				backend = ESPRESSO_BACKEND;
				net_idx = -1;
                yolo_net = nullptr;
			}
			~layerInfo_obj() {};

			precision_t precision;
			std::string layerName;
			std::vector<std::string> topLayerNames;
			std::vector<std::string> bottomLayerNames;
			espresso::layerType_t layerType;
			int numInputRows;
			int numInputCols;
			int inputDepth;
			int outputDepth;
			int dinFxPtLength;
			int dinNumFracBits;
			int whtFxPtLength;
			int whtNumFracBits;
			int doutFxPtLength;
			int doutNumFracBits;
			int biasFxPtLength;
			int biasNumFracBits;
			int leakyFxPtLength;
			int	leakyNumFracBits;
			int	numKernels = 1;
			int	kernelDepth = 1;
			int numKernelRows;
			int numKernelCols;
			int stride;
			int padding;
			bool globalPooling;
			int numFilterValues;
			float* flFilterData;
			float* flBiasData;
			fixedPoint_t* fxFilterData;
			fixedPoint_t* fxBiasData;
			int group;
			int localSize;
			float alpha;
			float flBeta;
			float fxBeta;
			espresso::activation_t activation;
			bool darknetNormScaleBias;
			bool darknetAct;
			int darknet_n_param;
			int darknet_classes_param;
			int darknet_outputs_param;
			int numKernelGroups;
			int numKrlPerGrp;
			int numKernelDepthGroups;
			backend_t backend;
			int net_idx;
			network *yolo_net;
	};


	// We can compute the spatial size of the output volume as a function of the input volume size (W), the
	// the receptive field size of the Conv Layer neurons (F), the stride with which they are applied (S), and
	// the amount of zero padding used (P) on the border.You can convince yourself that the correct formula for
	// calculating how many neurons fit is given by ((W - F + (2 * P)) / S) + 1. For example for a 7x7 input and a
	// 3x3 filter with stride 1 and pad 0 we would get a 5x5 output.With stride 2 we would get a 3x3 output.

	// Blob memory is row-major in layout, so the last / rightmost dimension changes fastest. For example, in a 4D blob, the value at index (n, k, h, w) is physically
	// located at index ((n * K + k) * H + h) * W + w. Number / N is the batch size of the data. Batch processing achieves better throughput for communication and device processing.
	// For an ImageNet training batch of 256 images N = 256. Channel / K is the feature dimension e.g. for RGB images K = 3.


	class Layer
	{
		public:
			Layer(espresso::layerInfo_obj* layerInfo);
			virtual ~Layer();
			virtual void ComputeLayer() = 0;
			virtual void ComputeLayer_FlPt() = 0;
			virtual void ComputeLayer_FxPt() = 0;
			virtual void ComputeLayerParam() = 0;

			espresso::precision_t m_precision;
			std::string m_layerName;
			std::vector<std::string> m_topLayerNames;
			std::vector<std::string> m_bottomLayerNames;
			espresso::layerType_t m_layerType;
			int m_numInputRows;
			int m_numInputCols;
			int m_inputDepth;
			int m_numOutputRows;
			int m_numOutputCols;
			int m_outputDepth;
			int m_dinFxPtLength;
			int m_dinNumFracBits;
			int m_whtFxPtLength;
			int m_whtNumFracBits;
			int m_doutFxPtLength;
			int m_doutNumFracBits;
			int m_biasFxPtLength;
			int m_biasNumFracBits;
			int m_leakyFxPtLength;
			int	m_leakyNumFracBits;
			int m_numKernelRows;
			int m_numKernelCols;
			int m_kernelDepth;
			int m_numKernels;
			int m_stride;
			int m_padding;
			bool m_fpga_upsample;
			bool m_fpga_activation;
			bool m_fpga_do_res_layer;
            bool m_fpga_do_res_layer_only;
			bool m_fpga_do_kernels1x1;
			bool m_fpga_krnl_1x1_layer;
			bool m_fpga_merged;
            std::vector<std::string> m_mergdArr;
			bool m_globalPooling;
			int m_numFilterValues;
			float* m_flFilterData;
			float* m_flBiasData;
			fixedPoint_t* m_fxFilterData;
			fixedPoint_t* m_fxBiasData;
			int m_group;
			int m_localSize;
			float m_alpha;
			float m_flBeta;
			float m_fxBeta;
			espresso::activation_t m_activation;
			bool m_darknetAct;
			int m_darknet_n_param;
			int m_darknet_classes_param;
			int m_darknet_outputs_param;
			bool m_darknetNormScaleBias;
			int m_numKernelGroups;
			int m_numKrlPerGrp;
			int m_numKernelDepthGroups;
			espresso::backend_t m_backend;
			int m_net_idx;
			network* m_yolo_net;
            bool m_fpga_do_1x1_res;
            bool m_fpga_do_res_1x1;

			espresso::Blob_t m_blob;
			std::vector<Layer*> m_topLayers;
			std::vector<Layer*> m_bottomLayers;
			int m_residualMapDepth;
			int m_numResidualMapRows;
			int m_numResidualMapsCols;
			float* m_residualMapData;
			FPGA_hndl* m_fpga_hndl;
			float* m_kernel1x1Data;
			int m_num1x1Kernels;
			int m_kernel1x1Depth;
			float* m_bias1x1Data;
			std::vector<int> m_merged_layers;
			int m_sequence_id;
			int m_base_bandwidth;
			int m_opt_bandwidth;
            double m_fpga_elapsed_time;
            double m_fpga_memPower;
			double m_fpga_avgIterTime;
	};
}

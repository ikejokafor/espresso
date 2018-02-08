#include <fcntl.h>
#include <stdint.h>
#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

// Google protobufs
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>


// caffe
#include "caffe.pb.h"

//	LayerTypes:
//		Input
//		Convolution
//		ReLU
//		Pooling
//		InnerProduct
//		Softmax
//		Concat

typedef struct {
	std::string layerName;
	std::vector<std::string> topLayerNames;
	std::vector<std::string> bottomLayerNames;
	std::string layerType;
	int numInputRows;
	int numInputCols;
	int inputDepth;
	int outputDepth;
	int numKernelRows;
	int numKernelCols;
	int stride;
	int padding;
	float *filterData;
	float *biasData;
} layerInfo_t;


std::vector<layerInfo_t> parseCaffeData(std::string protoFileName, std::string modelFileName);
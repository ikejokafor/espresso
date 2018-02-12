#include "caffeDataParser.hpp"
using namespace std;
using namespace caffe;
using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::Message;
using namespace caffeDataParser;

vector<layerInfo_t> parseCaffeData(string protoFileName, string modelFileName) {
	vector<layerInfo_t> caffeLayers;
    layerInfo_t layerInfo;
	NetParameter param;
	LayerParameter lparam;
    bool success;
    
    // for dcNet, might need to change for over networks


    // get Weights
    int fd = open((modelFileName).c_str(), O_RDONLY);
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(INT_MAX, 536870912);
    success = param.ParseFromCodedStream(coded_input);
    delete coded_input;
    delete raw_input;
    if(!success) {
        exit(1);
    }
    
    layerInfo.layerName = "data";
	layerInfo.layerType = "Input";
    layerInfo.inputDepth = param.input_dim(1);
	layerInfo.numInputRows = param.input_dim(2);
	layerInfo.numInputCols = param.input_dim(3);
    caffeLayers.push_back(layerInfo);
    

	for (int nlayers = 0; nlayers < param.layer_size(); nlayers++) {  

		layerInfo.layerName = " ";
		layerInfo.topLayerNames.clear();
		layerInfo.bottomLayerNames.clear();
		layerInfo.layerType = " ";
		layerInfo.numInputRows = 1;
		layerInfo.numInputCols = 1;
		layerInfo.inputDepth = 1;
		layerInfo.outputDepth = 1;
		layerInfo.numKernelRows = 1;
		layerInfo.numKernelCols = 1;
		layerInfo.stride = 1;
		layerInfo.padding = 0;
		layerInfo.filterData = NULL;
		layerInfo.biasData = NULL;

		lparam = param.layer(nlayers);
		layerInfo.layerName = lparam.name();
		layerInfo.layerType = lparam.type();
		for (int num_bottom_layers = 0; num_bottom_layers < lparam.bottom_size(); num_bottom_layers++) {   
			layerInfo.bottomLayerNames.push_back(lparam.bottom(num_bottom_layers));
		}
		for (int num_top_layers = 0; num_top_layers < lparam.top_size(); num_top_layers++) {
			layerInfo.topLayerNames.push_back(lparam.top(num_top_layers));
		}
		if (lparam.has_convolution_param()) {
			layerInfo.outputDepth = lparam.convolution_param().num_output();
			if(lparam.convolution_param().pad_size() > 0) {
                layerInfo.padding = lparam.convolution_param().pad(0);
            }
			layerInfo.numKernelRows = lparam.convolution_param().kernel_size(0);
            layerInfo.numKernelCols = lparam.convolution_param().kernel_size(0);
			if(lparam.convolution_param().stride_size() > 0) {
                layerInfo.stride = lparam.convolution_param().stride(0);
			}
            
            // filter weights
            layerInfo.filterData = (float*)malloc(lparam.blobs(0).data_size() * sizeof(float));    // asuming each number is a float
            for(int i = 0; i < lparam.blobs(0).data_size(); i++) {
                layerInfo.filterData[i] = lparam.blobs(0).data(i);
            }
            
            // bias
            layerInfo.biasData = (float*)malloc(lparam.blobs(1).data_size() * sizeof(float));    // asuming each number is a float
            for(int i = 0; i < lparam.blobs(1).data_size(); i++) {
                layerInfo.biasData[i] = lparam.blobs(1).data(i);
            }
		}
		if (lparam.has_lrn_param()) {
			//cout << "Local Size: " << lparam.lrn_param().local_size() << endl;
			//cout << "Alpha: " << lparam.lrn_param().alpha() << endl;
			//cout << "Beta: " << lparam.lrn_param().beta() << endl;
		}
		if (lparam.has_pooling_param()) {   // assuming pooling parameter is always MAX
			layerInfo.numKernelRows = lparam.pooling_param().kernel_size();
            layerInfo.numKernelCols = lparam.pooling_param().kernel_size();
            layerInfo.stride        = lparam.pooling_param().stride();
		}
		if (lparam.has_inner_product_param()) {
            layerInfo.outputDepth = lparam.inner_product_param().num_output();
		}
		if (lparam.has_dropout_param()) {
			//cout << "Dropout Ratio: " << lparam.dropout_param().dropout_ratio() << endl;
		}
        caffeLayers.push_back(layerInfo);
	}
    

	return caffeLayers;
}
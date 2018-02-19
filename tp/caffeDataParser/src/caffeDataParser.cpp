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


void GetLayerFilterAndBias(layerInfo_t *layerInfo, NetParameter wparam){
	LayerParameter lparam;
    V1LayerParameter v1lparam;
    
    if(wparam.layer_size() > 0) {
        for(int i = 0; i < wparam.layer_size(); i++) {
            lparam = wparam.layer(i);
            if(lparam.name() == layerInfo->layerName) {
                // filter weights
                layerInfo->filterData = (float*)malloc(lparam.blobs(0).data_size() * sizeof(float));    // asuming each number is a float
                for(int j = 0; j < lparam.blobs(0).data_size(); j++) {
                    layerInfo->filterData[j] = lparam.blobs(0).data(j);
                }
                
                // bias
                layerInfo->biasData = (float*)malloc(lparam.blobs(1).data_size() * sizeof(float));    // asuming each number is a float
                for(int j = 0; j < lparam.blobs(1).data_size(); j++) {
                    layerInfo->biasData[j] = lparam.blobs(1).data(j);
                }
                
                if(layerInfo->layerType != "InnerProduct") {
                    layerInfo->inputDepth = lparam.blobs(0).data_size() 
                        / lparam.convolution_param().num_output()   
                        / lparam.convolution_param().kernel_size(0) 
                        / lparam.convolution_param().kernel_size(0);
                }
                break;
            }
        }
    } else if (wparam.layers_size()){
        for(int i = 0; i < wparam.layers_size(); i++) {
            v1lparam = wparam.layers(i);
            if(v1lparam.name() == layerInfo->layerName) {
                // filter weights
                layerInfo->filterData = (float*)malloc(v1lparam.blobs(0).data_size() * sizeof(float));    // asuming each number is a float
                for(int j = 0; j < v1lparam.blobs(0).data_size(); j++) {
                    layerInfo->filterData[j] = v1lparam.blobs(0).data(j);
                }
                
                // bias
                layerInfo->biasData = (float*)malloc(v1lparam.blobs(1).data_size() * sizeof(float));    // asuming each number is a float
                for(int j = 0; j < v1lparam.blobs(1).data_size(); j++) {
                    layerInfo->biasData[j] = v1lparam.blobs(1).data(j);
                }
                
                if(layerInfo->layerType != "InnerProduct") {
                    layerInfo->inputDepth = v1lparam.blobs(0).data_size() 
                        / v1lparam.convolution_param().num_output()   
                        / v1lparam.convolution_param().kernel_size(0) 
                        / v1lparam.convolution_param().kernel_size(0);
                }
                break;
            }
        }   
    }
}


vector<layerInfo_t> parseCaffeData(string protoFileName, string modelFileName) {
	vector<layerInfo_t> caffeLayers;
    layerInfo_t layerInfo;
	NetParameter dparam;
    NetParameter wparam;
	LayerParameter lparam;
    V1LayerParameter v1lparam;
    

    // Open Deploy ProtoFile
    int fd = open((protoFileName).c_str(), O_RDONLY);
    if (fd == -1) {
        cout << protoFileName << " " << "not found" << endl;
        exit(1);
    }
    google::protobuf::io::FileInputStream* input = new google::protobuf::io::FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, &dparam);
    if(!success) {
        cout << "Error in Parsing Prototxt" << endl;
        exit(1);
    }
    
    
    
    // Open Binary ProtoFile
    fd = open((modelFileName).c_str(), O_RDONLY);
    if(fd == -1) {
        cout << modelFileName << " " << "not found" << endl;
    }
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(INT_MAX, 536870912);
    success = wparam.ParseFromCodedStream(coded_input);
    delete coded_input;
    delete raw_input;
    if(!success) {
        cout << "Error in Parsing Model File Prototxt" << endl;
        exit(1);
    }
    
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
    


    if(dparam.input_dim_size() > 0) {
        layerInfo.layerName = "data";
        layerInfo.layerType = "Input";
        layerInfo.inputDepth = dparam.input_dim(1);
        layerInfo.numInputRows = dparam.input_dim(2);
        layerInfo.numInputCols = dparam.input_dim(3);
        caffeLayers.push_back(layerInfo);
    }

    
    if(dparam.layer_size() > 0) {    // for new LayerParameter definition in proto definition
        for (int nlayers = 0; nlayers < dparam.layer_size(); nlayers++) {

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

            lparam = dparam.layer(nlayers);
            layerInfo.layerName = lparam.name();
            
            if(lparam.has_pooling_param()) {
                switch(lparam.pooling_param().pool()) {
                    case PoolingParameter_PoolMethod_MAX:
                        layerInfo.layerType = "Pooling_MAX";
                    break;                   
                    case PoolingParameter_PoolMethod_AVE:
                        layerInfo.layerType = "Pooling_AVE";
                    break;
                    default:                    
                    break;
                };  
            } else {
                layerInfo.layerType = lparam.type();
            }

            //if(caffeLayers[0].layerType == "Input" && layerInfo.layerType == "Input") {  // make this cleaner, to prevent two data layers for some prototxt          
            //    continue;
            //}
            
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
                
                GetLayerFilterAndBias(&layerInfo,  wparam);
            }
            if (lparam.has_lrn_param()) {
                layerInfo.localSize = lparam.lrn_param().local_size();
                layerInfo.alpha = lparam.lrn_param().alpha();
                layerInfo.beta = lparam.lrn_param().beta();
            }
            if (lparam.has_pooling_param()) {   // assuming pooling parameter is always MAX
                layerInfo.numKernelRows = lparam.pooling_param().kernel_size();
                layerInfo.numKernelCols = lparam.pooling_param().kernel_size();
                layerInfo.stride        = lparam.pooling_param().stride();
            }
            if (lparam.has_inner_product_param()) {
                layerInfo.outputDepth = lparam.inner_product_param().num_output();
                GetLayerFilterAndBias(&layerInfo,  wparam);
            }
            caffeLayers.push_back(layerInfo);
        }
    } else if(dparam.layers_size() > 0) { // for old V1LayerParameter definition in proto definition. might not need this anymore
        for (int nlayers = 0; nlayers < dparam.layers_size(); nlayers++) {  
           
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

            v1lparam = dparam.layers(nlayers);
            layerInfo.layerName = v1lparam.name();
            switch (v1lparam.type()){
                case V1LayerParameter_LayerType_DATA:
                    layerInfo.layerType = "Input";
                break;                
                case V1LayerParameter_LayerType_CONVOLUTION:
                    layerInfo.layerType = "Convolution";
                break;                               
                case V1LayerParameter_LayerType_RELU:
                    layerInfo.layerType = "ReLU";
                break;             
                case V1LayerParameter_LayerType_LRN:
                    layerInfo.layerType = "LRN";
                break;              
                case V1LayerParameter_LayerType_POOLING:
                    switch(v1lparam.pooling_param().pool()) {
                        case PoolingParameter_PoolMethod_MAX:
                            layerInfo.layerType = "Pooling_MAX";
                        break;
                        case PoolingParameter_PoolMethod_AVE:
                            layerInfo.layerType = "Pooling_AVE";
                        break;                        
                        default:                        
                        break;
                    };                   
                break;               
                case V1LayerParameter_LayerType_CONCAT:
                   layerInfo.layerType = "Concat";
                break;                
                case V1LayerParameter_LayerType_SPLIT:
                   layerInfo.layerType = "Split";
                break;
                case V1LayerParameter_LayerType_INNER_PRODUCT:
                    layerInfo.layerType = "InnerProduct";
                break;             
                case V1LayerParameter_LayerType_SOFTMAX:
                    layerInfo.layerType = "Softmax";
                break;      
                case V1LayerParameter_LayerType_SOFTMAX_LOSS:
                    layerInfo.layerType = "Softmax";
                break;  
                default:          
                break;
            }

            for (int num_bottom_layers = 0; num_bottom_layers < v1lparam.bottom_size(); num_bottom_layers++) {   
                layerInfo.bottomLayerNames.push_back(v1lparam.bottom(num_bottom_layers));
            }
            for (int num_top_layers = 0; num_top_layers < v1lparam.top_size(); num_top_layers++) {
                layerInfo.topLayerNames.push_back(v1lparam.top(num_top_layers));
            }
            if (v1lparam.has_convolution_param()) {
                layerInfo.outputDepth = v1lparam.convolution_param().num_output();
                if(v1lparam.convolution_param().pad_size() > 0) {
                    layerInfo.padding = v1lparam.convolution_param().pad(0);
                }
                layerInfo.numKernelRows = v1lparam.convolution_param().kernel_size(0);
                layerInfo.numKernelCols = v1lparam.convolution_param().kernel_size(0);
                if(v1lparam.convolution_param().stride_size() > 0) {
                    layerInfo.stride = v1lparam.convolution_param().stride(0);
                }              
                GetLayerFilterAndBias(&layerInfo, wparam);              
            }
            if (v1lparam.has_lrn_param()) {
                layerInfo.localSize = v1lparam.lrn_param().local_size();
                layerInfo.alpha = v1lparam.lrn_param().alpha();
                layerInfo.beta = v1lparam.lrn_param().beta();
            }
            if (v1lparam.has_pooling_param()) {   // assuming pooling parameter is always MAX
                layerInfo.numKernelRows = v1lparam.pooling_param().kernel_size();
                layerInfo.numKernelCols = v1lparam.pooling_param().kernel_size();
                layerInfo.stride        = v1lparam.pooling_param().stride();
            }
            if (v1lparam.has_inner_product_param()) {
                layerInfo.outputDepth = v1lparam.inner_product_param().num_output();
                GetLayerFilterAndBias(&layerInfo, wparam);
            }
            caffeLayers.push_back(layerInfo);
        }
    }
    
	return caffeLayers;
}


void printModelProtocalBuffer(string protoFileName, string modelFileName) {
	NetParameter param;
	LayerParameter *lparam;
    V1LayerParameter *v1lparam;
    
    int fd = open((modelFileName).c_str(), O_RDONLY);
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(INT_MAX, 536870912);
    bool success = param.ParseFromCodedStream(coded_input);
    delete coded_input;
    delete raw_input;
    if(!success) {
        cout << "Error in Parsing Prototxt" << endl;
        exit(1);
    }
    
    if(param.layer_size() > 0) {
        for (int nlayers = 0; nlayers < param.layer_size(); nlayers++) {  
            lparam = param.::caffe::NetParameter::mutable_layer(nlayers);
            if(lparam->has_convolution_param()) {
                lparam->clear_blobs();
            }
            if(lparam->has_inner_product_param()) {
                lparam->clear_blobs();
            }
        }
    } else if( param.layers_size()) {
        for (int nlayers = 0; nlayers < param.layers_size(); nlayers++) {  
            v1lparam = param.::caffe::NetParameter::mutable_layers(nlayers);
            if(v1lparam->has_convolution_param() || v1lparam->has_inner_product_param()) {
                v1lparam->clear_blobs();
            }
        }
    }
  
    fd = open((protoFileName).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    FileOutputStream* output = new FileOutputStream(fd);
    google::protobuf::TextFormat::Print(param, output);
    delete output;
    close(fd);
}
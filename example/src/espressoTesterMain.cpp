#include "Network.hpp"
#include "caffeDataParser.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace cv;

void dataTransform(vector<espresso::layerInfo_t<float>> &networkLayerInfo, vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo) {
    
    for(uint32_t i = 0; i < caffeDataParserLayerInfo.size(); i++) {
        networkLayerInfo[i].layerName             = caffeDataParserLayerInfo[i].layerName;     
        networkLayerInfo[i].topLayerNames         = caffeDataParserLayerInfo[i].topLayerNames;    
        networkLayerInfo[i].bottomLayerNames      = caffeDataParserLayerInfo[i].bottomLayerNames; 
        networkLayerInfo[i].layerType             = caffeDataParserLayerInfo[i].layerType;       
        networkLayerInfo[i].numInputRows          = caffeDataParserLayerInfo[i].numInputRows;     
        networkLayerInfo[i].numInputCols          = caffeDataParserLayerInfo[i].numInputCols;     
        networkLayerInfo[i].inputDepth            = caffeDataParserLayerInfo[i].inputDepth;       
        networkLayerInfo[i].outputDepth           = caffeDataParserLayerInfo[i].outputDepth;      
        networkLayerInfo[i].numKernelRows         = caffeDataParserLayerInfo[i].numKernelRows;    
        networkLayerInfo[i].numKernelCols         = caffeDataParserLayerInfo[i].numKernelCols;    
        networkLayerInfo[i].stride                = caffeDataParserLayerInfo[i].stride;          
        networkLayerInfo[i].padding               = caffeDataParserLayerInfo[i].padding;          
        networkLayerInfo[i].filterData            = caffeDataParserLayerInfo[i].filterData;       
        networkLayerInfo[i].biasData              = caffeDataParserLayerInfo[i].biasData;
        networkLayerInfo[i].localSize             = caffeDataParserLayerInfo[i].localSize;
        networkLayerInfo[i].alpha                 = caffeDataParserLayerInfo[i].alpha;
        networkLayerInfo[i].beta                  = caffeDataParserLayerInfo[i].beta;     
    }
}


void dataTransform(vector<espresso::layerInfo_t<FixedPoint>> &networkLayerInfo, vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo) {
    
    for(uint32_t i = 0; i < caffeDataParserLayerInfo.size(); i++) {
        networkLayerInfo[i].layerName             = caffeDataParserLayerInfo[i].layerName;     
        networkLayerInfo[i].topLayerNames         = caffeDataParserLayerInfo[i].topLayerNames;    
        networkLayerInfo[i].bottomLayerNames      = caffeDataParserLayerInfo[i].bottomLayerNames; 
        networkLayerInfo[i].layerType             = caffeDataParserLayerInfo[i].layerType;       
        networkLayerInfo[i].numInputRows          = caffeDataParserLayerInfo[i].numInputRows;     
        networkLayerInfo[i].numInputCols          = caffeDataParserLayerInfo[i].numInputCols;     
        networkLayerInfo[i].inputDepth            = caffeDataParserLayerInfo[i].inputDepth;       
        networkLayerInfo[i].outputDepth           = caffeDataParserLayerInfo[i].outputDepth;      
        networkLayerInfo[i].numKernelRows         = caffeDataParserLayerInfo[i].numKernelRows;    
        networkLayerInfo[i].numKernelCols         = caffeDataParserLayerInfo[i].numKernelCols;    
        networkLayerInfo[i].stride                = caffeDataParserLayerInfo[i].stride;          
        networkLayerInfo[i].padding               = caffeDataParserLayerInfo[i].padding;
        networkLayerInfo[i].localSize             = caffeDataParserLayerInfo[i].localSize;
        networkLayerInfo[i].alpha                 = caffeDataParserLayerInfo[i].alpha;
        networkLayerInfo[i].beta                  = caffeDataParserLayerInfo[i].beta;       
        if(caffeDataParserLayerInfo[i].layerType == "Convolution" || caffeDataParserLayerInfo[i].layerType == "InnerProduct") {
            networkLayerInfo[i].filterData = new FixedPoint [    
                                                                caffeDataParserLayerInfo[i].numKernelRows 
                                                                * caffeDataParserLayerInfo[i].numKernelCols 
                                                                * caffeDataParserLayerInfo[i].inputDepth 
                                                                * caffeDataParserLayerInfo[i].outputDepth
                                                            ];
            int numFilterValues = caffeDataParserLayerInfo[i].numKernelRows 
                                  * caffeDataParserLayerInfo[i].numKernelCols
                                  * caffeDataParserLayerInfo[i].inputDepth 
                                  * caffeDataParserLayerInfo[i].outputDepth;           
            networkLayerInfo[i].biasData = new FixedPoint [caffeDataParserLayerInfo[i].outputDepth];
            for(int j = 0; j < numFilterValues; j++) {
                networkLayerInfo[i].filterData[j] = caffeDataParserLayerInfo[i].filterData[j];
                networkLayerInfo[i].filterData[j].SetParam(18,10);
            }
            for(int j = 0; j < caffeDataParserLayerInfo[i].outputDepth; i++) {
                networkLayerInfo[i].biasData[j] = caffeDataParserLayerInfo[i].filterData[j];
                networkLayerInfo[i].biasData[j].SetParam(18,10);
            }
        }
    }
}


int main(int argc, char **argv) {
    if(argc < 4) {
        cout << "Need more args" << endl;
        exit(0);
    }
    
    // OverFeat

    // SSD
    
    // YOLO
    
    // VGG16
    // ../../../../../caffe-master/models/vgg16/vgg16_deploy.prototxt ../../../../../caffe-master/models/vgg16/VGG_ILSVRC_16_layers.caffemodel ../../../scripts/image.png data prob 
    
    // VGG19
    // ../../../../../caffe-master/models/vgg19/vgg16_deploy.prototxt ../../../../../caffe-master/models/vgg19/VGG_ILSVRC_16_layers.caffemodel ../../../scripts/image.png data prob 
    
    // GoogleNet
    // ../../../../../caffe-master/models/bvlc_googlenet/deploy.prototxt ../../../../../caffe-master/models/bvlc_googlenet/bvlc_googlenet.caffemodel ../../../scripts/image.png data prob 
    
    // Alexnet
    // ../../../../../caffe-master/models/bvlc_alexnet/deploy.prototxt ../../../../../caffe-master/models/bvlc_alexnet/bvlc_alexnet.caffemodel ../../../scripts/image.png data prob 

    // dcNet
    // ../../../../../caffe-master/models/dcNet/deploy_sqz_2.prototxt ../../../../../caffe-master/models/dcNet/sqz_rework_iter_100000.caffemodel ../../../../../detector_test_kitti/temp.png data objectness0_soft 

    
    string protoTxt = argv[1];
    string model = argv[2];
    Mat img = imread(argv[3], IMREAD_COLOR);
    
    //printModelProtocalBuffer(argv[1], argv[2]);
    //exit(0);
    
    // Read in model
    ofstream fd;
    Blob_t<float> inputBlob;
	vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo = parseCaffeData(protoTxt, model);
    vector<espresso::layerInfo_t<float>> networkLayerInfo;
    networkLayerInfo.resize(caffeDataParserLayerInfo.size());
    dataTransform(networkLayerInfo, caffeDataParserLayerInfo);
	Network<float> *network = new Network<float>(networkLayerInfo);
    

    // Input Image, for dcNet remember to subtract 127 from each pixel value to compare results
	inputBlob.data = new float[img.channels() * img.rows * img.cols]; 
    inputBlob.depth = img.channels();
    inputBlob.numRows = img.rows;
    inputBlob.numCols = img.cols;    
    for(int c = 0; c < img.channels(); c++) {
        for(int i=0; i < img.rows; i++) {
            for(int j=0; j < img.cols; j++) { 
                int a = img.at<cv::Vec3b>(i,j)[c];
                index3D(img.channels(), img.rows, img.cols, inputBlob.data, c, i, j) = (float)a;
            }
        }
    }  
    network->m_cnn[0]->m_inputDepth     = inputBlob.depth;
    network->m_cnn[0]->m_numInputRows   = inputBlob.numRows;
    network->m_cnn[0]->m_numInputCols   = inputBlob.numCols;
    network->m_cnn[0]->m_blob.data      = inputBlob.data;
    
    string beginLayer = " "; 
    string endlayer = " ";
    
    if(argc == 5) {
        beginLayer = argv[4];
    }
    if(argc == 6) {
        endlayer = argv[5];
    }
    
    network->Forward(beginLayer, endlayer);
    
    int layerIdx = network->ReturnLayerIdx(endlayer);
    if(layerIdx != -1) {
        fd.open("output.txt");
        if(network->m_cnn[layerIdx]->m_blob.depth > 1 && network->m_cnn[layerIdx]->m_blob.numRows > 1 && network->m_cnn[layerIdx]->m_blob.numCols > 1) {
            for(int i = 0; i < network->m_cnn[layerIdx]->m_blob.depth; i++) {
                for(int j = 0; j < network->m_cnn[layerIdx]->m_blob.numRows; j++) {
                    for(int k = 0; k < network->m_cnn[layerIdx]->m_blob.numCols; k++) {
                        fd << index3D(  
                                        network->m_cnn[layerIdx]->m_blob.depth, 
                                        network->m_cnn[layerIdx]->m_blob.numRows, 
                                        network->m_cnn[layerIdx]->m_blob.numCols, 
                                        network->m_cnn[layerIdx]->m_blob.data, i, j, k
                                     ) << " ";
                    }
                    fd << endl;
                }
                fd << endl << endl << endl;
            }
        } else {
            for(int i = 0; i < network->m_cnn[layerIdx]->m_blob.depth; i++) {
                fd << network->m_cnn[layerIdx]->m_blob.data[i] << endl;
            }
        }
    }
    



    //fd.open("obj.txt");
    //for(int i = 0; i < network->m_outputLayers[3]->m_blob.depth; i++) {
    //    for(int j = 0; j < network->m_outputLayers[3]->m_blob.numRows; j++) {
    //        for(int k = 0; k < network->m_outputLayers[3]->m_blob.numCols; k++) {
    //            fd << index3D(network->m_outputLayers[3]->m_blob.depth, network->m_outputLayers[3]->m_blob.numRows, network->m_outputLayers[3]->m_blob.numCols, network->m_outputLayers[3]->m_blob.data, i, j, k) << " ";
    //        }
    //        fd << endl;
    //    }
    //    fd << endl << endl << endl;
    //}
    //fd.close();
    //
    //fd.open("cls.txt");
    //for(int i = 0; i < network->m_outputLayers[4]->m_blob.depth; i++) {
    //    for(int j = 0; j < network->m_outputLayers[4]->m_blob.numRows; j++) {
    //        for(int k = 0; k < network->m_outputLayers[4]->m_blob.numCols; k++) {
    //            fd << index3D(network->m_outputLayers[4]->m_blob.depth, network->m_outputLayers[4]->m_blob.numRows, network->m_outputLayers[4]->m_blob.numCols, network->m_outputLayers[4]->m_blob.data, i, j, k) << " ";
    //        }
    //        fd << endl;
    //    }
    //    fd << endl << endl << endl;
    //}
    //fd.close();
    //
    //fd.open("loc.txt");
    //for(int i = 0; i < network->m_outputLayers[5]->m_blob.depth; i++) {
    //    for(int j = 0; j < network->m_outputLayers[5]->m_blob.numRows; j++) {
    //        for(int k = 0; k < network->m_outputLayers[5]->m_blob.numCols; k++) {
    //            fd << index3D(network->m_outputLayers[5]->m_blob.depth, network->m_outputLayers[5]->m_blob.numRows, network->m_outputLayers[5]->m_blob.numCols, network->m_outputLayers[5]->m_blob.data, i, j, k) << " ";
    //        }
    //        fd << endl;
    //    }
    //    fd << endl << endl << endl;
    //}
    //fd.close();  
    
    delete network;
  
    return 0;
}
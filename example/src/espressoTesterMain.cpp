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
        networkLayerInfo[i].layerName               = caffeDataParserLayerInfo[i].layerName;     
        networkLayerInfo[i].topLayerNames           = caffeDataParserLayerInfo[i].topLayerNames;    
        networkLayerInfo[i].bottomLayerNames        = caffeDataParserLayerInfo[i].bottomLayerNames; 
        networkLayerInfo[i].layerType               = caffeDataParserLayerInfo[i].layerType;       
        networkLayerInfo[i].numInputRows            = caffeDataParserLayerInfo[i].numInputRows;     
        networkLayerInfo[i].numInputCols            = caffeDataParserLayerInfo[i].numInputCols;     
        networkLayerInfo[i].inputDepth              = caffeDataParserLayerInfo[i].inputDepth;       
        networkLayerInfo[i].outputDepth             = caffeDataParserLayerInfo[i].outputDepth;      
        networkLayerInfo[i].numKernelRows           = caffeDataParserLayerInfo[i].numKernelRows;    
        networkLayerInfo[i].numKernelCols           = caffeDataParserLayerInfo[i].numKernelCols;    
        networkLayerInfo[i].stride                  = caffeDataParserLayerInfo[i].stride;          
        networkLayerInfo[i].padding                 = caffeDataParserLayerInfo[i].padding;
        networkLayerInfo[i].globalPooling           = caffeDataParserLayerInfo[i].globalPooling;
        networkLayerInfo[i].group                   = caffeDataParserLayerInfo[i].group;
        networkLayerInfo[i].localSize               = caffeDataParserLayerInfo[i].localSize;
        networkLayerInfo[i].alpha                   = caffeDataParserLayerInfo[i].alpha;
        networkLayerInfo[i].beta                    = caffeDataParserLayerInfo[i].beta;     
        if(caffeDataParserLayerInfo[i].layerType == "Convolution" || caffeDataParserLayerInfo[i].layerType == "InnerProduct") {
            networkLayerInfo[i].filterData = (float*)malloc(    caffeDataParserLayerInfo[i].numFilterValues
                                                                * sizeof(float)
                                                           );
            memcpy  (   networkLayerInfo[i].filterData, 
                        caffeDataParserLayerInfo[i].filterData,
                        caffeDataParserLayerInfo[i].numFilterValues                                 
                        * sizeof(float)                    
                    );

            networkLayerInfo[i].biasData = (float*)malloc(  caffeDataParserLayerInfo[i].numBiasValues 
                                                            * sizeof(float)
                                                         );
            memcpy  (   networkLayerInfo[i].biasData, 
                        caffeDataParserLayerInfo[i].biasData, 
                        caffeDataParserLayerInfo[i].numBiasValues
                        * sizeof(float)
                    );
        } else {
            networkLayerInfo[i].filterData = NULL;
            networkLayerInfo[i].biasData = NULL;
        }
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
        networkLayerInfo[i].group                 = caffeDataParserLayerInfo[i].group;   
        networkLayerInfo[i].globalPooling         = caffeDataParserLayerInfo[i].globalPooling;        
        if(caffeDataParserLayerInfo[i].layerType == "Convolution" || caffeDataParserLayerInfo[i].layerType == "InnerProduct") {
            networkLayerInfo[i].filterData = new FixedPoint[caffeDataParserLayerInfo[i].numFilterValues]; 
            for(int j = 0; j < caffeDataParserLayerInfo[i].numFilterValues; j++) {
                networkLayerInfo[i].filterData[j] = caffeDataParserLayerInfo[i].filterData[j];
                networkLayerInfo[i].filterData[j].SetParam(32, 16);
            }        
            
            networkLayerInfo[i].biasData = new FixedPoint[caffeDataParserLayerInfo[i].numBiasValues];
            for(int j = 0; j < caffeDataParserLayerInfo[i].numBiasValues; j++) {
                networkLayerInfo[i].biasData[j] = caffeDataParserLayerInfo[i].biasData[j];
                networkLayerInfo[i].biasData[j].SetParam(32, 16);
            }
            
        } else {
            networkLayerInfo[i].filterData = NULL;
            networkLayerInfo[i].biasData = NULL;
        }
    }
}


int main(int argc, char **argv) {
    
    // OverFeat

    //SSD
    
    // YOLO
    
    // Squeezenet
    // ./espressoTester ../../../../../caffe-master/models/squeezenet/deploy.prototxt ../../../../../caffe-master/models/squeezenet/squeezenet_v1.1.caffemodel ../../../scripts/image.png data prob
    
    // VGG16
    // ./espressoTester ../../../../../caffe-master/models/vgg16/vgg16_deploy.prototxt ../../../../../caffe-master/models/vgg16/VGG_ILSVRC_16_layers.caffemodel ../../../scripts/image.png data prob 
    
    // VGG19
    // ./espressoTester ../../../../../caffe-master/models/vgg19/vgg19_deploy.prototxt ../../../../../caffe-master/models/vgg19/VGG_ILSVRC_19_layers.caffemodel ../../../scripts/image.png data prob 
    
    // GoogleNet
    // ./espressoTester ../../../../../caffe-master/models/bvlc_googlenet/deploy.prototxt ../../../../../caffe-master/models/bvlc_googlenet/bvlc_googlenet.caffemodel ../../../scripts/image.png data prob 
    
    // Alexnet
    // ./espressoTester ../../../../../caffe-master/models/bvlc_alexnet/deploy.prototxt ../../../../../caffe-master/models/bvlc_alexnet/bvlc_alexnet.caffemodel ../../../scripts/image.png data prob 

    // dcNet
    // ./espressoTester ../../../../../caffe-master/models/dcNet/deploy_sqz_2.prototxt ../../../../../caffe-master/models/dcNet/sqz_rework_iter_100000.caffemodel ../../../../../detector_test_kitti/temp.png data objectness0_soft 
    
    // printModelProtocalBuffer(argv[1], argv[2]);
    // exit(0);
       
    if(argc < 4) {
        cout << "Need more args" << endl;
        exit(0);
    }
       
    string protoTxt = argv[1];
    string model = argv[2];
    Mat img = imread(argv[3], IMREAD_COLOR);
    
    string beginLayer = " "; 
    string endLayer = " ";  
    if(argc >= 5) {
        beginLayer = argv[4];
    }
    if(argc == 6) {
        endLayer = argv[5];
    }
   
    // Read in model
    Blob_t<float> inputBlob;
    Blob_t<FixedPoint> inputBlob_fxPt;
    vector<espresso::layerInfo_t<float>> networkLayerInfo;
    vector<espresso::layerInfo_t<FixedPoint>> networkLayerInfo_fxPt;
    
	vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo = parseCaffeData(protoTxt, model);
    
    networkLayerInfo.resize(caffeDataParserLayerInfo.size());
    networkLayerInfo_fxPt.resize(caffeDataParserLayerInfo.size());
    
    dataTransform(networkLayerInfo, caffeDataParserLayerInfo);
    dataTransform(networkLayerInfo_fxPt, caffeDataParserLayerInfo);
    
	Network<float> *network = new Network<float>(networkLayerInfo);
    Network<FixedPoint> *network_fxPt = new Network<FixedPoint>(networkLayerInfo_fxPt);
    for(uint32_t i = 0; i < caffeDataParserLayerInfo.size(); i++) {
        if(caffeDataParserLayerInfo[i].layerType == "Convolution" || caffeDataParserLayerInfo[i].layerType == "InnerProduct") {
            free(caffeDataParserLayerInfo[i].filterData);
            free(caffeDataParserLayerInfo[i].biasData);
        }
    }

    int beginLayerIdx = network->ReturnLayerIdx(beginLayer);   
    int endLayerIdx = network->ReturnLayerIdx(endLayer);
    ofstream fd;
    
    if(endLayerIdx != -1 && beginLayerIdx != -1) {   
        // // Input Image, for dcNet remember to subtract 127 from each pixel value to compare results (float)
        // cout << "Doing Floating Point CNN" << endl;
        // inputBlob.data = new float[img.channels() * img.rows * img.cols]; 
        // inputBlob.depth = img.channels();
        // inputBlob.numRows = img.rows;
        // inputBlob.numCols = img.cols;    
        // for(int c = 0; c < img.channels(); c++) {
        //     for(int i=0; i < img.rows; i++) {
        //         for(int j = 0; j < img.cols; j++) { 
        //             int a = img.at<cv::Vec3b>(i,j)[c];
        //             //index3D(img.channels(), img.rows, img.cols, inputBlob.data, c, i, j) = (float)a - 127.0f;   // for dcNet
        //             index3D(img.channels(), img.rows, img.cols, inputBlob.data, c, i, j) = (float)a;
        //         }
        //     }
        // }  
        // network->m_cnn[0]->m_inputDepth     = inputBlob.depth;
        // network->m_cnn[0]->m_numInputRows   = inputBlob.numRows;
        // network->m_cnn[0]->m_numInputCols   = inputBlob.numCols;
        // network->m_cnn[0]->m_blob.data      = inputBlob.data;
        // network->Forward(beginLayer, endLayer);
        //
        // fd.open("output_espresso.txt");
        // if(network->m_cnn[endLayerIdx]->m_blob.depth > 1 && network->m_cnn[endLayerIdx]->m_blob.numRows > 1 && network->m_cnn[endLayerIdx]->m_blob.numCols > 1) {
        //     for(int i = 0; i < network->m_cnn[endLayerIdx]->m_blob.depth; i++) {
        //         for(int j = 0; j < network->m_cnn[endLayerIdx]->m_blob.numRows; j++) {
        //             for(int k = 0; k < network->m_cnn[endLayerIdx]->m_blob.numCols; k++) {
        //                 fd << index3D(  
        //                                 network->m_cnn[endLayerIdx]->m_blob.depth, 
        //                                 network->m_cnn[endLayerIdx]->m_blob.numRows, 
        //                                 network->m_cnn[endLayerIdx]->m_blob.numCols, 
        //                                 network->m_cnn[endLayerIdx]->m_blob.data, i, j, k
        //                              ) << " ";
        //             }
        //             fd << endl;
        //         }
        //         fd << endl << endl << endl;
        //     }
        // } else {
        //     for(int i = 0; i < network->m_cnn[endLayerIdx]->m_blob.depth; i++) {
        //         fd << network->m_cnn[endLayerIdx]->m_blob.data[i] << endl;
        //     }
        // }
        // fd.close();
        
        
        // Input Image, for dcNet remember to subtract 127 from each pixel value to compare results (fixedPoint)
        cout << "Doing Fixed Point CNN" << endl;
        inputBlob_fxPt.data = new FixedPoint[img.channels() * img.rows * img.cols]; 
        inputBlob_fxPt.depth = img.channels();
        inputBlob_fxPt.numRows = img.rows;
        inputBlob_fxPt.numCols = img.cols;    
        for(int c = 0; c < img.channels(); c++) {
            for(int i=0; i < img.rows; i++) {
                for(int j = 0; j < img.cols; j++) { 
                    int a = img.at<cv::Vec3b>(i,j)[c];
                    //float b = (float)a - 127.0f; // for dcNet
                    float b = (float)a;
                    index3D(img.channels(), img.rows, img.cols, inputBlob_fxPt.data, c, i, j) = (float)b;
                    index3D(img.channels(), img.rows, img.cols, inputBlob_fxPt.data, c, i, j).SetParam(32, 16);
                }
            }
        } 
        network_fxPt->m_cnn[0]->m_inputDepth     = inputBlob_fxPt.depth;
        network_fxPt->m_cnn[0]->m_numInputRows   = inputBlob_fxPt.numRows;
        network_fxPt->m_cnn[0]->m_numInputCols   = inputBlob_fxPt.numCols;
        network_fxPt->m_cnn[0]->m_blob.data      = inputBlob_fxPt.data;
        network_fxPt->Forward(beginLayer, endLayer);
    
        fd.open("output_espresso_fxPt.txt");
        if(network_fxPt->m_cnn[endLayerIdx]->m_blob.depth > 1 && network_fxPt->m_cnn[endLayerIdx]->m_blob.numRows > 1 && network_fxPt->m_cnn[endLayerIdx]->m_blob.numCols > 1) {
            for(int i = 0; i < network_fxPt->m_cnn[endLayerIdx]->m_blob.depth; i++) {
                for(int j = 0; j < network_fxPt->m_cnn[endLayerIdx]->m_blob.numRows; j++) {
                    for(int k = 0; k < network_fxPt->m_cnn[endLayerIdx]->m_blob.numCols; k++) {
                        fd << index3D(  
                                        network_fxPt->m_cnn[endLayerIdx]->m_blob.depth, 
                                        network_fxPt->m_cnn[endLayerIdx]->m_blob.numRows, 
                                        network_fxPt->m_cnn[endLayerIdx]->m_blob.numCols, 
                                        network_fxPt->m_cnn[endLayerIdx]->m_blob.data, i, j, k
                                     ).toFloat() << " ";
                    }
                    fd << endl;
                }
                fd << endl << endl << endl;
            }
        } else {
            for(int i = 0; i < network_fxPt->m_cnn[endLayerIdx]->m_blob.depth; i++) {
                fd << network_fxPt->m_cnn[endLayerIdx]->m_blob.data[i].toFloat() << endl;
            }
        }
        fd.close();
    }
    

    // DcNet Output
    // fd.open("obj.txt");
    // for(int i = 0; i < network->m_outputLayers[3]->m_blob.depth; i++) {
    //     for(int j = 0; j < network->m_outputLayers[3]->m_blob.numRows; j++) {
    //         for(int k = 0; k < network->m_outputLayers[3]->m_blob.numCols; k++) {
    //             fd << index3D(network->m_outputLayers[3]->m_blob.depth, network->m_outputLayers[3]->m_blob.numRows, network->m_outputLayers[3]->m_blob.numCols, network->m_outputLayers[3]->m_blob.data, i, j, k) << " ";
    //         }
    //         fd << endl;
    //     }
    //     fd << endl << endl << endl;
    // }
    // fd.close();
    // 
    // fd.open("cls.txt");
    // for(int i = 0; i < network->m_outputLayers[4]->m_blob.depth; i++) {
    //     for(int j = 0; j < network->m_outputLayers[4]->m_blob.numRows; j++) {
    //         for(int k = 0; k < network->m_outputLayers[4]->m_blob.numCols; k++) {
    //             fd << index3D(network->m_outputLayers[4]->m_blob.depth, network->m_outputLayers[4]->m_blob.numRows, network->m_outputLayers[4]->m_blob.numCols, network->m_outputLayers[4]->m_blob.data, i, j, k) << " ";
    //         }
    //         fd << endl;
    //     }
    //     fd << endl << endl << endl;
    // }
    // fd.close();
    // 
    // fd.open("loc.txt");
    // for(int i = 0; i < network->m_outputLayers[5]->m_blob.depth; i++) {
    //     for(int j = 0; j < network->m_outputLayers[5]->m_blob.numRows; j++) {
    //         for(int k = 0; k < network->m_outputLayers[5]->m_blob.numCols; k++) {
    //             fd << index3D(network->m_outputLayers[5]->m_blob.depth, network->m_outputLayers[5]->m_blob.numRows, network->m_outputLayers[5]->m_blob.numCols, network->m_outputLayers[5]->m_blob.data, i, j, k) << " ";
    //         }
    //         fd << endl;
    //     }
    //     fd << endl << endl << endl;
    // }
    // fd.close();  
    
    delete network;
    delete network_fxPt;
  
    return 0;
}
#include "Network.hpp"
#include "caffeDataParser.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace cv;

void dataTransform(vector<espresso::layerInfo_t> &networkLayerInfo, vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo) {
    
    for(int i = 0; i < caffeDataParserLayerInfo.size(); i++) {
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
    }
}


int main(int argc, char **agrv) {
    
    ofstream fd;
	string protoTxt = "/home/ikenna/caffe-master/models/dcNet/deploy_sqz_2.prototxt";
    string model = "/home/ikenna/caffe-master/models/dcNet/dcNet_deploy_sq_2.caffemodel";
	vector<caffeDataParser::layerInfo_t> caffeDataParserLayerInfo = parseCaffeData(protoTxt, model);
    vector<espresso::layerInfo_t> networkLayerInfo;
    networkLayerInfo.resize(caffeDataParserLayerInfo.size());
    dataTransform(networkLayerInfo, caffeDataParserLayerInfo);
	Network *network = new Network(networkLayerInfo);
    
    Blob_t inputBlob;
    Mat img = imread("/home/ikenna/detector_test_kitti/temp.png", IMREAD_COLOR);
	inputBlob.data = (float*)malloc(img.channels() * img.rows * img.cols * sizeof(float)); 
    inputBlob.depth = img.channels();
    inputBlob.numRows = img.rows;
    inputBlob.numCols = img.cols;    
    for(int c = 0; c < img.channels(); c++) {
        for(int i=0; i < img.rows; i++) { //210
            for(int j=0; j < img.cols; j++) { // 695
                int a = img.at<cv::Vec3b>(i,j)[c];
                index3D(img.channels(), img.rows, img.cols, inputBlob.data, c, i, j) = (float)a - 127.0f;
            }
        }
    }  
    network->m_cnn[0]->m_inputDepth     = inputBlob.depth;
    network->m_cnn[0]->m_numInputRows   = inputBlob.numRows;
    network->m_cnn[0]->m_numInputCols   = inputBlob.numCols;
    network->m_cnn[0]->m_blob.data      = inputBlob.data;
    

    network->Forward();
    
    fd.open("obj.txt");
    for(int i = 0; i < network->m_outputLayers[0]->m_blob.depth; i++) {
        for(int j = 0; j < network->m_outputLayers[0]->m_blob.numRows; j++) {
            for(int k = 0; k < network->m_outputLayers[0]->m_blob.numCols; k++) {
                fd << index3D(network->m_outputLayers[0]->m_blob.depth, network->m_outputLayers[0]->m_blob.numRows, network->m_outputLayers[0]->m_blob.numCols, network->m_outputLayers[0]->m_blob.data, i, j, k) << " ";
            }
            fd << endl;
        }
        fd << endl << endl << endl;
    }
    fd.close();
    
    fd.open("cls.txt");
    for(int i = 0; i < network->m_outputLayers[1]->m_blob.depth; i++) {
        for(int j = 0; j < network->m_outputLayers[1]->m_blob.numRows; j++) {
            for(int k = 0; k < network->m_outputLayers[1]->m_blob.numCols; k++) {
                fd << index3D(network->m_outputLayers[1]->m_blob.depth, network->m_outputLayers[1]->m_blob.numRows, network->m_outputLayers[1]->m_blob.numCols, network->m_outputLayers[1]->m_blob.data, i, j, k) << " ";
            }
            fd << endl;
        }
        fd << endl << endl << endl;
    }
    fd.close();
    
    fd.open("loc.txt");
    for(int i = 0; i < network->m_outputLayers[2]->m_blob.depth; i++) {
        for(int j = 0; j < network->m_outputLayers[2]->m_blob.numRows; j++) {
            for(int k = 0; k < network->m_outputLayers[2]->m_blob.numCols; k++) {
                fd << index3D(network->m_outputLayers[2]->m_blob.depth, network->m_outputLayers[2]->m_blob.numRows, network->m_outputLayers[2]->m_blob.numCols, network->m_outputLayers[2]->m_blob.data, i, j, k) << " ";
            }
            fd << endl;
        }
        fd << endl << endl << endl;
    }
    fd.close();  

    delete network;
    
    return 0;
}
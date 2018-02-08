#include "Network.hpp"
using namespace std;

int main(int argc, char **agrv) {
	string protoTxt = "/home/ikenna/caffe-master/models/dcNet/deploy_sqz.prototxt";
	vector<layerInfo_t> layerInfo = parseCaffeData(protoTxt, " ");
	Network *network = new Network(layerInfo);
	Blob_t inputBlob;
	inputBlob.data = (float*)malloc(network->m_cnn[0]->m_inputDepth * network->m_cnn[0]->m_numInputRows * network->m_cnn[0]->m_numInputCols * sizeof(float));
	Blob_t outputBlob;
	outputBlob.data = (float*)malloc	(
											network->m_cnn[network->m_cnn.size() - 1]->m_outputDepth 
											* network->m_cnn[network->m_cnn.size() - 1]->m_numOutputRows 
											* network->m_cnn[network->m_cnn.size() - 1]->m_numOutputCols 
											* sizeof(float)
										);
	network->Forward(inputBlob, &outputBlob);


    return 0;
}
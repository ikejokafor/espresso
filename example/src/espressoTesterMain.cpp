#include "Network.hpp"
using namespace std;

int main(int argc, char **agrv) {
	string protoTxt = "/home/ikenna/SOC_IT/espresso/dep/caffeDataParser/models/dcNet/deploy_sqz.prototxt";
	vector<layerInfo_t> layerInfo = parseCaffeData(protoTxt, " ");
	Network *network = new Network(layerInfo);
	Blob_t inputBlob;
	Blob_t outputBlob;
	network->Forward(inputBlob, &outputBlob);


    return 0;
}
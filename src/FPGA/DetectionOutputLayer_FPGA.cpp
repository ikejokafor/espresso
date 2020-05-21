#include "DetectionOutputLayer_FPGA.hpp"
using namespace std;


DetectionOutputLayer_FPGA::DetectionOutputLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


DetectionOutputLayer_FPGA::~DetectionOutputLayer_FPGA() { }


void DetectionOutputLayer_FPGA::ComputeLayer()
{
	if (m_bottomLayers[0]->m_precision == espresso::FLOAT)
	{
		int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
		fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
		float        *flData = m_bottomLayers[0]->m_blob.flData;
		for (int i = 0; i < blobSize; i++)
		{
			fxData[i] = fixedPoint::create(m_dinFxPtLength, m_dinNumFracBits, flData[i]);
		}
	}
	std::cout << m_layerName << " Merged" << endl;
}


void DetectionOutputLayer_FPGA::ComputeLayer_FlPt() { }


void DetectionOutputLayer_FPGA::ComputeLayer_FxPt()
{

}



void DetectionOutputLayer_FPGA::ComputeLayerParam()
{

}

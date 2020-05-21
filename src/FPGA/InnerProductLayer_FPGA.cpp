#include "InnerProductLayer_FPGA.hpp"
using namespace std;


InnerProductLayer_FPGA::InnerProductLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


InnerProductLayer_FPGA::~InnerProductLayer_FPGA() { }


void InnerProductLayer_FPGA::ComputeLayer()
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


void InnerProductLayer_FPGA::ComputeLayer_FlPt() { }


void InnerProductLayer_FPGA::ComputeLayer_FxPt()
{

}


void InnerProductLayer_FPGA::ComputeLayerParam()
{

}

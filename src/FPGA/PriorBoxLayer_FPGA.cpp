#include "PriorBoxLayer_FPGA.hpp"
using namespace std;


PriorBoxLayer_FPGA::PriorBoxLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


PriorBoxLayer_FPGA::~PriorBoxLayer_FPGA() { }


void PriorBoxLayer_FPGA::ComputeLayer()
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


void PriorBoxLayer_FPGA::ComputeLayer_FlPt() { }


void PriorBoxLayer_FPGA::ComputeLayer_FxPt()
{

}


void PriorBoxLayer_FPGA::ComputeLayerParam()
{

}

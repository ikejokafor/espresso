#include "PSROIPoolingLayer_FPGA.hpp"
using namespace std;


PSROIPoolingLayer_FPGA::PSROIPoolingLayer_FPGA(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo) { }


PSROIPoolingLayer_FPGA::~PSROIPoolingLayer_FPGA() { }


void PSROIPoolingLayer_FPGA::ComputeLayer()
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


void PSROIPoolingLayer_FPGA::ComputeLayer_FlPt() { }


void PSROIPoolingLayer_FPGA::ComputeLayer_FxPt()
{

}


void PSROIPoolingLayer_FPGA::ComputeLayerParam()
{

}

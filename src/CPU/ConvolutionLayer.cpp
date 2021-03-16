#include "ConvolutionLayer.hpp"


ConvolutionLayer::ConvolutionLayer(espresso::layerInfo_obj* layerInfo) : Layer(layerInfo)  { }


ConvolutionLayer::~ConvolutionLayer() { }


void ConvolutionLayer::ComputeLayer()
{
    if(m_precision == espresso::FLOAT)
    {
	    ComputeLayer_FlPt();
    }
	else if(m_precision == espresso::FIXED)
    {
	    ComputeLayer_FxPt();
    }
}


void ConvolutionLayer::ComputeLayer_FlPt()
{
	if (m_bottomLayers[0]->m_precision == espresso::FIXED)
    {
        int dinNumFracBits   = m_bottomLayers[0]->m_dinNumFracBits;
        int blobSize         = m_bottomLayers[0]->m_blob.blobSize;
        fixedPoint_t *fxData = m_bottomLayers[0]->m_blob.fxData;
        float        *flData = m_bottomLayers[0]->m_blob.flData;
        for(int i = 0; i < blobSize; i++) {
            flData[i] = fixedPoint::toFloat(dinNumFracBits, fxData[i]);
        }
    }
    float *datain = m_bottomLayers[0]->m_blob.flData;
    int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
    int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
    float *dataout = m_topLayers[0]->m_blob.flData;
    float *filters = m_flFilterData;
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	std::vector<std::thread> threads(nthreads);
	for (int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
		{
			for (int m = bi_m; m < ei_m; m++) {
				for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride)
				{
					for(int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride)
					{
						int do_i = index3D(m_numOutputRows, m_numOutputCols, m, x, y);
						dataout[do_i] = m_flBiasData[m];
						for (int k = 0; k < m_kernelDepth; k++)
						{
							for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++)
							{
								for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++)
								{
									if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) // in valid region, assuming zero padding
									{
										int do_i = index3D(m_numOutputRows, m_numOutputCols, m, x, y);
										int di_i = index3D(numInputBlobRows, numInputBlobCols, k, i, j);
										int f_i = index4D(m_kernelDepth, m_numKernelRows, m_numKernelCols, m, k, kr, kc);
										dataout[do_i] += (datain[di_i] * filters[f_i]);
									}
								}
							}
						}
					}
				}
			}
		}, t * m_numKernels / nthreads, (t + 1) == nthreads ? m_numKernels : (t + 1) * m_numKernels / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
	// need this code for group parameter and iterate over groups while changing these pointers
	// dataout += ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
	// datain  += ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols);
	// filters += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);
	if(m_darknetAct)
	{
		if (m_activation == espresso::LEAKY)
		{
			LeakyActivation_FlPt();
		}
	}
}


void ConvolutionLayer::ComputeLayer_FxPt()
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
	fixedPoint_t *datain = m_bottomLayers[0]->m_blob.fxData;
	int numInputBlobRows = m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = m_bottomLayers[0]->m_blob.numCols;
	fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
	fixedPoint_t *filters = m_fxFilterData;
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	std::vector<std::thread> threads(nthreads);
	for (int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
        {
            for (int m = bi_m; m < ei_m; m++) 
            {
                for (int x = 0, a = 0; x < m_numOutputRows; x++, a += m_stride)
                {
                    for (int y = 0, b = 0; y < m_numOutputCols; y++, b += m_stride)
                    {
                        int do_i = index3D(m_numOutputRows, m_numOutputCols, m, x, y);
                        dataout[do_i] = m_flBiasData[m];
                        for (int k = 0; k < m_kernelDepth; k++)
                        {
                            for (int i = a - m_padding, kr = 0; kr < m_numKernelRows; i++, kr++)
                            {
                                for (int j = b - m_padding, kc = 0; kc < m_numKernelCols; j++, kc++)
                                {
                                    if ((i >= 0 && j >= 0) && (i < numInputBlobRows && j < numInputBlobCols)) // in valid region, assuming zero padding
                                    {
                                        int do_i = index3D(m_numOutputRows, m_numOutputCols, m, x, y);
                                        int di_i = index3D(numInputBlobRows, numInputBlobCols, k, i, j);
                                        int f_i = index4D(m_kernelDepth, m_numKernelRows, m_numKernelCols, m, k, kr, kc);
                                        dataout[do_i] += (datain[di_i] * filters[f_i]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        },
        t * m_numKernels / nthreads,
        (t + 1) == nthreads ? m_numKernels : (t + 1) * m_numKernels / nthreads,
        t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
	// need this code for group parameter and iterate over groups while changing these pointers
	// dataout += ((m_outputDepth / m_group) * m_numOutputRows * m_numOutputCols);
	// datain  += ((inputBlobDepth / m_group) * numInputBlobRows * numInputBlobCols);
	// filters += ((m_numKernels / m_group) * m_kernelDepth * m_numKernelRows * m_numKernelCols);
    fixedPoint::SetParam(
		m_biasFxPtLength,
		m_biasNumFracBits,
		m_doutFxPtLength,
		m_doutNumFracBits,
		m_topLayers[0]->m_blob.fxData,
		m_topLayers[0]->m_blob.blobSize);
	if (m_darknetAct)
	{
		if (m_activation == espresso::LEAKY)
		{
			LeakyActivation_FxPt();
		}
	}
}


void ConvolutionLayer::ComputeLayerParam()
{
	m_inputDepth = m_bottomLayers[0]->m_outputDepth;
	m_numInputRows = m_bottomLayers[0]->m_numOutputRows;
	m_numInputCols = m_bottomLayers[0]->m_numOutputCols;
	m_numOutputRows = (int)((m_numInputRows - m_numKernelRows + 2 * m_padding) / m_stride) + 1;
	m_numOutputCols = (int)((m_numInputCols - m_numKernelCols + 2 * m_padding) / m_stride) + 1;
	m_numKernels = m_outputDepth;
	m_kernelDepth = m_inputDepth / m_group;
	m_blob.depth = m_outputDepth;
	m_blob.numRows = m_numOutputRows;
	m_blob.numCols = m_numOutputCols;
	m_blob.blobSize = m_outputDepth * m_numOutputRows * m_numOutputCols;
	m_blob.flData =  new float[m_outputDepth * m_numOutputRows * m_numOutputCols];
	m_blob.fxData = new fixedPoint_t[m_outputDepth * m_numOutputRows * m_numOutputCols];
}


void ConvolutionLayer::LeakyActivation_FlPt()
{
	float *dataout = m_topLayers[0]->m_blob.flData;
	int numValues = m_outputDepth * m_numOutputRows * m_numOutputCols;
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	std::vector<std::thread> threads(nthreads);
	for (int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
			{
				for (int m = bi_m; m < ei_m; m++)
				{
					dataout[m] = (dataout[m] < 0.0f) ? dataout[m] * 0.1f : dataout[m];
				}
			},
			t * numValues / nthreads,
			(t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads,
			t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}


void ConvolutionLayer::LeakyActivation_FxPt() {
    fixedPoint_t *dataout = m_topLayers[0]->m_blob.fxData;
	int numValues = m_outputDepth * m_numOutputRows * m_numOutputCols;
#ifdef HIGH_PERF
	int nthreads = std::thread::hardware_concurrency();
#else
	int nthreads = 1;
#endif
	std::vector<std::thread> threads(nthreads);
	fixedPoint_t leakyScaleValue = fixedPoint::create(m_leakyFxPtLength, m_leakyNumFracBits, 0.1f);
	fixedPoint_t one = fixedPoint::create(m_leakyFxPtLength, m_leakyNumFracBits, 1.0f);
	int resFxPtLength  = m_dinFxPtLength + m_leakyFxPtLength;
    int resNumFracBits = m_dinNumFracBits + m_leakyNumFracBits;
	for (int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
		{
			for (int m = bi_m; m < ei_m; m++)
			{
				dataout[m] = (dataout[m] < 0.0f) ? dataout[m] * leakyScaleValue : dataout[m] * one;
			}
		}, t * numValues / nthreads, (t + 1) == nthreads ? numValues : (t + 1) * numValues / nthreads, t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
	// Leaky value is always less than 1 so just normalize back to din params
	fixedPoint::SetParam(
		resFxPtLength,
		resNumFracBits,
		m_dinFxPtLength,
		m_dinNumFracBits,
		m_topLayers[0]->m_blob.fxData,
		m_topLayers[0]->m_blob.blobSize
	);
}

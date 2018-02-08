#ifndef __LAYER_H__
#define __LAYER_H__

#define index2D(ROW, COL, A, x, y)                            A[y * COL + x]
#define index3D(DEPTH0, ROW, COL, A, x, y, z)                 A[(z * ROW + y) * COL + x]
#define index4D(DEPTH1, DEPTH0, ROW, COL, A, x, y, z0, z1)    A[((z1 * DEPTH1 + z0) * ROW + y) * COL + x]

#include <string>
#include <cstring>
#include <vector>
#include <random>
#include <float.h> 

typedef struct {
    float *data;
	int numRows;
    int numCols;
	int depth;
} Blob_t;


// We can compute the spatial size of the output volume as a function of the input volume size (W), the
// the receptive field size of the Conv Layer neurons (F), the stride with which they are applied (S), and
// the amount of zero padding used (P) on the border.You can convince yourself that the correct formula for 
// calculating how many neurons “fit” is given by ((W - F + (2 * P)) / S) + 1. For example for a 7x7 input and a 
// 3x3 filter with stride 1 and pad 0 we would get a 5x5 output.With stride 2 we would get a 3x3 output.

class Layer {

    public:
        Layer   (  
					std::string layerName = " ",
					std::vector<std::string> topLayerNames = std::vector<std::string>(),
					std::vector<std::string> bottomLayerNames = std::vector<std::string>(),
					std::string  layerType = " ",
					int numInputRows = 1,
					int numInputCols = 1,
					int inputDepth = 1,
					int outputDepth = 1,
					int numKernelRows = 1,
					int numKernelCols = 1,
					int stride = 1,
					int padding = 0,
					float *filterData = NULL,
					float *biasData = NULL
                );
		virtual ~Layer();

        virtual void ComputeLayer(Blob_t inputBlob) = 0;
		virtual void ComputeLayerParam() = 0;

		std::string m_layerName;
		std::vector<std::string> m_topLayerNames;
		std::vector<std::string> m_bottomLayerNames;
		std::string m_layerType;
		int m_numInputRows;
		int m_numInputCols;
		int m_inputDepth;
		int m_numOutputRows;
		int m_numOutputCols;
		int m_outputDepth;
		int m_numKernelRows;
		int m_numKernelCols;
		int m_kernelDepth;
		int m_numKernels;
		int m_stride;
		int m_padding;
		float *m_filterData;
		float *m_biasData;
		Blob_t m_blob;
		std::vector<Layer*> m_topLayers;
		std::vector<Layer*> m_bottomLayers;

    protected:


    private:

};

#endif
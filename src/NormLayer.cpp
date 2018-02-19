#include "NormLayer.hpp"
using namespace std;


template <typename DType>
NormLayer<DType>::NormLayer	(
                                string layerName,
                                vector<string> topLayerNames,
                                vector<string> bottomLayerNames,
                                string layerType,
                                int numInputRows,
                                int numInputCols,
                                int inputDepth,
                                int outputDepth,
                                int numKernelRows,
                                int numKernelCols,
                                int stride,
                                int padding,
                                DType *filterData,
                                DType *biasData,
                                int localSize,
                                float alpha,
                                float beta,
                                int length,
                                int numFracbits
                            ) : Layer<DType>	(	
                                                    layerName,
                                                    topLayerNames,
                                                    bottomLayerNames,
                                                    layerType,
                                                    numInputRows,
                                                    numInputCols,
                                                    inputDepth,
                                                    outputDepth,
                                                    numKernelRows,
                                                    numKernelCols,
                                                    stride,
                                                    padding,
                                                    filterData,
                                                    biasData,
                                                    localSize,
                                                    alpha,
                                                    beta,
                                                    length,
                                                    numFracbits
                                                ) {
}


template <typename DType>
NormLayer<DType>::~NormLayer() {}


template <typename DType>
void NormLayer<DType>::ComputeLayerParam() {
	// input size
	this->m_inputDepth   = this->m_bottomLayers[0]->m_outputDepth;
	this->m_numInputRows = this->m_bottomLayers[0]->m_numOutputRows;
	this->m_numInputCols = this->m_bottomLayers[0]->m_numOutputCols;

	// output size
	this->m_outputDepth = this->m_inputDepth;
	this->m_numOutputRows = this->m_numInputRows;
	this->m_numOutputCols = this->m_numInputCols;
    
    // create output blob
	this->m_blob.depth = this->m_outputDepth;
	this->m_blob.numRows = this->m_numOutputRows;
	this->m_blob.numCols = this->m_numOutputCols;
	this->m_blob.data = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
}


template <typename DType>
void NormLayer<DType>::ComputeLayer() {


	// get input
	DType *datain = this->m_bottomLayers[0]->m_blob.data;
	int numInputBlobRows = this->m_bottomLayers[0]->m_blob.numRows;
	int numInputBlobCols = this->m_bottomLayers[0]->m_blob.numCols;
	int inputBlobDepth = this->m_bottomLayers[0]->m_blob.depth;

    
	// output
	DType *dataout = this->m_topLayers[0]->m_blob.data;
 
 
    // get padded x_i squared
    DType *paddedSquare = (DType*)malloc((this->m_inputDepth + this->m_localSize - 1) * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
    memset(paddedSquare, 0, ((this->m_inputDepth + this->m_localSize - 1) * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType)));
    for(int i = 0; i < ((this->m_inputDepth + this->m_localSize - 1) * this->m_numOutputRows * this->m_numOutputCols); i++) {   // dont really need to square the pad values, because they are zero
        paddedSquare[i] = datain[i] * datain[i];
    }
    
    
    // compute norm values

    DType *normValues = (DType*)malloc(this->m_outputDepth * this->m_numOutputRows * this->m_numOutputCols * sizeof(DType));
    int paddedSquareDepth = (this->m_inputDepth + this->m_localSize - 1);
	for (int m = 0, start = 0; m < this->m_outputDepth; m++, start++) {
		for (int x = 0; x < this->m_numOutputRows; x++) {
			for(int y = 0; y < this->m_numOutputCols; y++) {
                float sum = 0.0f;
                for(int i = start; i < (this->m_localSize + start); i++) {
                    sum += index3D(paddedSquareDepth, numInputBlobRows, numInputBlobCols, paddedSquare, i, x, y);
                }
                index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, normValues, m, x, y) = pow((1.0f + (this->m_alpha / (float)this->m_localSize) * sum), this->m_beta);
			}
		}
	}
    
    //FILE *fd = fopen("tmp.txt", "w");    
    //for(int i = 0; i < (this->m_inputDepth * this->m_numOutputRows * this->m_numOutputCols); i++) {
    //    fprintf(fd, "%f\n", index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, normValues, m, x, y));
    //}
    //fclose(fd);
    //exit(0);

   
    // normalize
	for (int m = 0; m < this->m_outputDepth; m++) {
		for (int x = 0; x < this->m_numOutputRows; x++) {
			for(int y = 0; y < this->m_numOutputCols; y++) {
				index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, dataout, m, x, y) 
                    = index3D(inputBlobDepth, numInputBlobRows, numInputBlobCols, datain, m, x, y)
                        / index3D(this->m_outputDepth, this->m_numOutputRows, this->m_numOutputCols, normValues, m, x, y);
			}
		}
	}

}


template <>
void NormLayer<FixedPoint>::ComputeLayer() {



}


template class NormLayer<float>;
template class NormLayer<FixedPoint>;

#include "CNN_Network.hpp"
using namespace std;


espresso::CNN_Network::CNN_Network(vector<espresso::layerInfo_obj*> layerInfoArr, vector<int> &outputLayers) {

    for(int i = 0; i < layerInfoArr.size(); i++)
    {
        if(layerInfoArr[i]->layerType == espresso::INPUT)
        {
            m_cnn.push_back(new DataLayer(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::CONCAT && layerInfoArr[i]->backend == espresso::ESPRESSO_BACKEND)
        {
            m_cnn.push_back(new ConcatLayer(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::CONCAT && layerInfoArr[i]->backend == espresso::DARKNET_BACKEND)
        {
            m_cnn.push_back(new ConcatLayer_DKN(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::CONCAT && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new ConcatLayer_FPGA(layerInfoArr[i]));
        }
        else if (layerInfoArr[i]->layerType == espresso::CONVOLUTION && layerInfoArr[i]->backend == espresso::ESPRESSO_BACKEND)
        {
            m_cnn.push_back(new ConvolutionLayer(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::CONVOLUTION && layerInfoArr[i]->backend == espresso::DARKNET_BACKEND)
        {
            m_cnn.push_back(new ConvolutionLayer_DKN(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::CONVOLUTION && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new ConvolutionLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::UPSAMPLE && layerInfoArr[i]->backend == espresso::ESPRESSO_BACKEND)
        {
            m_cnn.push_back(new UpSampleLayer(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::UPSAMPLE && layerInfoArr[i]->backend == espresso::DARKNET_BACKEND)
        {
            m_cnn.push_back(new UpSampleLayer_DKN(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::UPSAMPLE && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new UpSampleLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::RESIDUAL && layerInfoArr[i]->backend == espresso::ESPRESSO_BACKEND)
        {
            m_cnn.push_back(new ResidualLayer(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::RESIDUAL && layerInfoArr[i]->backend == espresso::DARKNET_BACKEND)
        {
            m_cnn.push_back(new ResidualLayer_DKN(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::RESIDUAL  && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new ResidualLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::YOLO)
        {
            m_cnn.push_back(new YOLOLayer(layerInfoArr[i]));
        }
        else
        {
            cout << "[ESPRESSO]: " << "Skipped Loading Layer: " << layerInfoArr[i]->layerName << endl;
        }
    }
    for(int i = 0; i < m_cnn.size(); i++)
    {
        cout << "[ESPRESSO]: Loaded Layer " << i <<  " " << m_cnn[i]->m_layerName << endl;
    }
    GetTopAndBottomLayers();
    // GetOutputLayers(outputLayers);
}



espresso::CNN_Network::~CNN_Network()
{
    for(int i = 0; i < m_cnn.size(); i++)
    {
        delete m_cnn[i];
    }
    free_network(m_cnn[0]->m_yolo_net);
}


void espresso::CNN_Network::GetTopAndBottomLayers()
{
    for(int i = 0; i < m_cnn.size() ; i++) // for every layer
       {
           for (uint32_t j = 0; j < m_cnn[i]->m_topLayerNames.size(); j++) // for every top layer of the current cnn layer
           {
               for (uint32_t k = 0; k < m_cnn.size(); k++) // search for the top layer: where I write my data to
               {
                if (m_cnn[i]->m_topLayerNames[j] == m_cnn[k]->m_layerName)
                {
                    m_cnn[i]->m_topLayers.push_back(m_cnn[k]);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < m_cnn.size(); i++) // for every layer
    {
        for (uint32_t j = 0; j < m_cnn[i]->m_bottomLayerNames.size(); j++) // for every bottom layer of the current cnn layer
        {
            for (uint32_t k = 0; k < m_cnn.size(); k++)  // search for the bottom layer: where I get my data from
            {
                if (m_cnn[i]->m_bottomLayerNames[j] == m_cnn[k]->m_layerName)
                {
                    m_cnn[i]->m_bottomLayers.push_back(m_cnn[k]);
                    break;
                }
            }
        }
    }
}


void espresso::CNN_Network::GetOutputLayers(vector<int> &outputLayers)
{
    if(outputLayers.size() != 0)
    {
        for(int i = 0; i < outputLayers.size(); i++)
        {
            m_outputLayers.push_back(m_cnn[outputLayers[i]]);
        }
    }
    else
    {
        stack<espresso::Layer*> outputLayers_tmp;
        for(uint32_t i = 0; i < m_cnn.size(); i ++) {
            outputLayers_tmp.push(m_cnn[i]);
            for(uint32_t j = 0; j < m_cnn.size(); j++)
            {
                for(uint32_t k = 0; k < m_cnn[j]->m_bottomLayers.size(); k++)
                {
                    if(m_cnn[i]->m_layerName == m_cnn[j]->m_bottomLayers[k]->m_layerName)
                    {
                        outputLayers_tmp.pop();
                        goto label0;
                    }
                }
            }
            label0:
            continue;
        }
        // reverse order
        m_outputLayers.resize(outputLayers.size());
        m_outputLayers[outputLayers.size() - 1] = outputLayers_tmp.top();
        outputLayers_tmp.pop();
        for(uint32_t i = outputLayers.size() - 1; ((int)i) >= 0; i--)
        {
            m_outputLayers[i] = outputLayers_tmp.top();
            outputLayers_tmp.pop();
        }
    }
}


void espresso::CNN_Network::getBgnEndLayer(int& startIdx, string start, int& endIdx, string end)
{
    if(start == "")
    {
        startIdx = 0;
    }
    if(end == "")
    {
        endIdx = m_cnn.size();
    }
    if(startIdx < 0 || endIdx < 0) {
        for(uint32_t i = 0; i < m_cnn.size(); i++){
            if(m_cnn[i]->m_layerName == start) {
                startIdx = i;
            }
            if(m_cnn[i]->m_layerName == end) {
                endIdx = i;
            }
        }
    }
    if((startIdx == -1 || endIdx == -1) || (endIdx < startIdx)) {
        cout << "[ESPRESSO]: No start layer: " << start << " or end layer: " << end  << " or direction wrong" << endl;
        exit(0);
    }
}


void espresso::CNN_Network::cfgFPGALayers(string mrgFmt_fn)
{
    ifstream infile(mrgFmt_fn);
    string line;
    while(getline(infile, line))
    {
        string sequence;
        stringstream check1(line);
        vector<string> sequence_tokens;
        while(getline(check1, sequence, ','))
        {
            sequence_tokens.push_back(sequence);
        }
        int seqID = stoi(sequence_tokens[0]);
        string SW_layer = sequence_tokens[1];
        int i = findLayer(SW_layer);
        m_cnn[i]->m_fpga_merged = true;
        m_cnn[i]->m_sequence_id = seqID;
        vector<string> tmp(sequence_tokens.begin() + 2, sequence_tokens.end());
        m_cnn[i]->m_mergdArr = tmp;
    }
    /*
    int sequence_id = 0;
    for(int i = (endIdx - 2); i >= startIdx; i--)
    {
        if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows > 1 && m_cnn[i]->m_darknetAct)
        {
            m_cnn[i]->m_fpga_activation = true;
        }
        if((i + 1) < endIdx && m_cnn[i + 1]->m_layerType == UPSAMPLE)
        {
            m_cnn[i]->m_fpga_upsample = true;
        }
        // Standalone 1x1
        if((i - 1) >= 0)
        {
            espresso::Layer* layer = m_cnn[i];
            if( m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1
                && ((m_cnn[i - 1]->m_layerType != CONVOLUTION || m_cnn[i - 1]->m_numKernelRows <= 1)
                    && m_cnn[i - 1]->m_layerType != RESIDUAL)
            ) {
                m_cnn[i]->m_kernel1x1Data           = m_cnn[i]->m_flFilterData;
                m_cnn[i]->m_bias1x1Data             = m_cnn[i]->m_flBiasData;
                m_cnn[i]->m_num1x1Kernels           = m_cnn[i]->m_numKernels;
                m_cnn[i]->m_kernel1x1Depth          = m_cnn[i]->m_kernelDepth;
                m_cnn[i]->m_fpga_do_kernel1x1       = true;
                m_cnn[i]->m_fpga_krnl_1x1_layer     = true;
            }
        }
        // SW followed by Res followed by 1x1
        if((i - 1) >= 0 && (i - 2) >= 0)
        {
            if( m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1
                && m_cnn[i - 1]->m_layerType == RESIDUAL
                && m_cnn[i - 2]->m_layerType == CONVOLUTION && m_cnn[i - 2]->m_numKernelRows > 1
            ) {
                m_cnn[i]->m_fpga_merged_1x1 = true;
                // Residual
                m_cnn[i - 2]->m_fpga_do_res_layer   = true;
                m_cnn[i - 2]->m_residualMapDepth    = m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth;
                m_cnn[i - 2]->m_numResidualMapRows  = m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows;
                m_cnn[i - 2]->m_numResidualMapsCols = m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols;
                m_cnn[i - 2]->m_residualMapData     = m_cnn[i - 1]->m_bottomLayers[0]->m_blob.flData;
                // 1x1 Kernel
                m_cnn[i - 2]->m_fpga_do_kernel1x1   = true;
                m_cnn[i - 2]->m_num1x1Kernels       = m_cnn[i]->m_numKernels;
                m_cnn[i - 2]->m_kernel1x1Depth      = m_cnn[i]->m_kernelDepth;
                m_cnn[i - 2]->m_kernel1x1Data       = m_cnn[i]->m_flFilterData;
                m_cnn[i - 2]->m_bias1x1Data         = m_cnn[i]->m_flBiasData;
                m_cnn[i - 2]->m_sequence_id         = sequence_id;
                m_cnn[i - 1]->m_sequence_id         = sequence_id;
                m_cnn[i]->m_sequence_id             = sequence_id;
                sequence_id++;
                m_cnn[i - 2]->m_base_bandwidth = (
                    + m_cnn[i - 2]->m_numKernels * m_cnn[i - 2]->m_kernelDepth * m_cnn[i - 2]->m_numKernelRows * m_cnn[i - 2]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 2]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 2]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 2]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i - 2]->m_topLayers[0]->m_blob.depth * m_cnn[i - 2]->m_topLayers[0]->m_blob.numRows * m_cnn[i - 2]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_bottomLayers.size() * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_topLayers[0]->m_blob.depth * m_cnn[i - 1]->m_topLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_numKernels * m_cnn[i]->m_kernelDepth * m_cnn[i]->m_numKernelRows * m_cnn[i]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i]->m_bottomLayers[0]->m_blob.depth * m_cnn[i]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                m_cnn[i - 2]->m_opt_bandwidth = (
                    m_cnn[i - 2]->m_numKernels * m_cnn[i - 2]->m_kernelDepth * m_cnn[i - 2]->m_numKernelRows * m_cnn[i - 2]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 2]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 2]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 2]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + (m_cnn[i - 1]->m_bottomLayers.size() - 1) * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_numKernels * m_cnn[i]->m_kernelDepth * m_cnn[i]->m_numKernelRows * m_cnn[i]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                i -= 2;
            }

        }
        // SW followed by 1x1
        if(i - 1 > 0)
        {
            if( m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1
                && m_cnn[i - 1]->m_layerType == CONVOLUTION && m_cnn[i - 1]->m_numKernelRows > 1
            ) {
                m_cnn[i]->m_fpga_merged_1x1         = true;
                m_cnn[i - 1]->m_fpga_do_kernel1x1   = true;
                m_cnn[i - 1]->m_num1x1Kernels       = m_cnn[i]->m_numKernels;
                m_cnn[i - 1]->m_kernel1x1Depth      = m_cnn[i]->m_kernelDepth;
                m_cnn[i - 1]->m_kernel1x1Data       = m_cnn[i]->m_flFilterData;
                m_cnn[i - 1]->m_bias1x1Data         = m_cnn[i]->m_flBiasData;
                m_cnn[i - 1]->m_sequence_id         = sequence_id;
                m_cnn[i]->m_sequence_id             = sequence_id;
                sequence_id++;
                m_cnn[i - 1]->m_base_bandwidth = (
                    m_cnn[i - 1]->m_numKernels * m_cnn[i - 1]->m_kernelDepth * m_cnn[i - 1]->m_numKernelRows * m_cnn[i - 1]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_topLayers[0]->m_blob.depth * m_cnn[i - 1]->m_topLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_numKernels * m_cnn[i]->m_kernelDepth * m_cnn[i]->m_numKernelRows * m_cnn[i]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i]->m_bottomLayers[0]->m_blob.depth * m_cnn[i]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                m_cnn[i - 1]->m_opt_bandwidth = (
                    m_cnn[i - 1]->m_numKernels * m_cnn[i - 1]->m_kernelDepth * m_cnn[i - 1]->m_numKernelRows * m_cnn[i - 1]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_numKernels * m_cnn[i]->m_kernelDepth * m_cnn[i]->m_numKernelRows * m_cnn[i]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                i--;
            }
        }
        // SW followed by Res
        if(i - 1 > 0)
        {
            if( m_cnn[i]->m_layerType == RESIDUAL
                && m_cnn[i - 1]->m_layerType == CONVOLUTION && m_cnn[i - 1]->m_numKernelRows > 1
            ) {
                m_cnn[i - 1]->m_fpga_do_res_layer   = true;
                m_cnn[i - 1]->m_residualMapDepth    = m_cnn[i]->m_bottomLayers[0]->m_blob.depth;
                m_cnn[i - 1]->m_numResidualMapRows  = m_cnn[i]->m_bottomLayers[0]->m_blob.numRows;
                m_cnn[i - 1]->m_numResidualMapsCols = m_cnn[i]->m_bottomLayers[0]->m_blob.numCols;
                m_cnn[i - 1]->m_residualMapData     = m_cnn[i]->m_bottomLayers[0]->m_blob.flData;
                m_cnn[i - 1]->m_sequence_id         = sequence_id;
                m_cnn[i]->m_sequence_id             = sequence_id;
                sequence_id++;
                m_cnn[i - 1]->m_base_bandwidth = (
                    m_cnn[i - 1]->m_numKernels * m_cnn[i - 1]->m_kernelDepth * m_cnn[i - 1]->m_numKernelRows * m_cnn[i - 1]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_topLayers[0]->m_blob.depth * m_cnn[i - 1]->m_topLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_bottomLayers.size() * m_cnn[i]->m_bottomLayers[0]->m_blob.depth * m_cnn[i]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                m_cnn[i - 1]->m_opt_bandwidth = (
                    m_cnn[i - 1] ->m_numKernels *m_cnn[i - 1]->m_kernelDepth * m_cnn[i - 1]->m_numKernelRows * m_cnn[i - 1]->m_numKernelCols * PIXEL_SIZE
                    + m_cnn[i - 1]->m_bottomLayers[0]->m_blob.depth * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i - 1]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + (m_cnn[i]->m_bottomLayers.size() - 1) * m_cnn[i]->m_bottomLayers[0]->m_blob.depth * m_cnn[i]->m_bottomLayers[0]->m_blob.numRows * m_cnn[i]->m_bottomLayers[0]->m_blob.numCols * PIXEL_SIZE
                    + m_cnn[i]->m_topLayers[0]->m_blob.depth * m_cnn[i]->m_topLayers[0]->m_blob.numRows * m_cnn[i]->m_topLayers[0]->m_blob.numCols * PIXEL_SIZE
                );
                i--;
            }
        }
    }
    */
}


void espresso::CNN_Network::Forward(string start, string end)
{
    // find start and end
    int startIdx = -1;
    int endIdx = -1;
    getBgnEndLayer(startIdx, start, endIdx, end);
    for (int i = startIdx; i < endIdx; i++)
    {
        m_cnn[i]->ComputeLayerParam();
    }
    // print FPGA stats
    for (int i = startIdx; i < endIdx; i++)
    {
        // cout << m_cnn[i]->m_layerName << "    " << m_cnn[i]->m_sequence_id << endl;
        if(m_cnn[i]->m_sequence_id != -1 && m_cnn[i]->m_base_bandwidth != 0)
        {
            cout << "Sequence" + to_string(m_cnn[i]->m_sequence_id) << "," << m_cnn[i]->m_base_bandwidth << "," << m_cnn[i]->m_opt_bandwidth << endl;
        }
    }
    // Forward Propagation
    for(int i = 0; i < endIdx; i++)
    {
        if(m_cnn[i]->m_layerType == espresso::YOLO) continue;
        printLayerStats(i);
        cout << "[ESPRESSO]: Processing Layer" << " " << m_cnn[i]->m_layerName << endl;
        m_cnn[i]->ComputeLayer();
        cout << "[ESPRESSO]: Finished Layer" << " " << m_cnn[i]->m_layerName << endl;
    }
}


int espresso::CNN_Network::ReturnLayerIdx(string name)
{
    for(uint32_t i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerName == name)
        {
            return i;
        }
    }
    return -1;
}


void espresso::CNN_Network::printLayerStats(int i)
{
    // cout << "Layer " << i <<  " " << m_cnn[i]->m_layerName << endl;
    // cout << "\t inputDepth:     \t\t"   << m_cnn[i]->m_inputDepth       << endl;
    // cout << "\t numInputRows:   \t\t"   << m_cnn[i]->m_numInputRows     << endl;
    // cout << "\t numInputCols:   \t\t"   << m_cnn[i]->m_numInputCols     << endl;
    // cout << "\t outputDepth:    \t\t"   << m_cnn[i]->m_outputDepth      << endl;
    // cout << "\t numOutputRows:  \t\t"   << m_cnn[i]->m_numOutputRows    << endl;
    // cout << "\t numOutputCols:  \t\t"   << m_cnn[i]->m_numOutputCols    << endl;
    // if(m_cnn[i]->m_precision == espresso::FLOAT)
    // {
    // 	cout << "\t precision:  \t\t\t"   << "FLOAT"                      << endl;
    // }
    // else
    // {
    // 	cout << "\t precision:  \t\t\t"   << "FIXED"                      << endl;
    // }
    // if(m_cnn[i]->m_layerType == "Convolution" || m_cnn[i]->m_layerType == "InnerProduct")
    // {
    // 	cout << "\t Stride:             \t\t" << m_cnn[i]->m_stride << endl;
    // 	cout << "\t Padding:            \t\t" << m_cnn[i]->m_padding << endl;
    // 	cout << "\t Number of Kernels:  \t\t" << m_cnn[i]->m_numKernels << endl;
    // 	cout << "\t Kernel Depth:       \t\t" << m_cnn[i]->m_kernelDepth << endl;
    // 	cout << "\t Kernel Size:        \t\t" << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
    // }
    // cout << "\t dinFxPtLength:   \t\t"   << m_cnn[i]->m_dinFxPtLength     << endl;
    // cout << "\t dinNumFracBits:  \t\t"   << m_cnn[i]->m_dinNumFracBits    << endl;
    // cout << "\t whtFxPtLength:   \t\t"   << m_cnn[i]->m_whtFxPtLength     << endl;
    // cout << "\t whtNumFracBits:  \t\t"   << m_cnn[i]->m_whtNumFracBits    << endl;
    // cout << "\t doutFxPtLength:  \t\t"   << m_cnn[i]->m_doutFxPtLength    << endl;
    // cout << "\t doutNumFracBits: \t\t"   << m_cnn[i]->m_doutNumFracBits   << endl;
}


void espresso::CNN_Network::setHardware(FPGA_hndl* fpga_hndl)
{
    for (int i = 0; i < m_cnn.size(); i++)
    {
        m_cnn[i]->m_fpga_hndl = fpga_hndl;
    }
}


 int espresso::CNN_Network::findLayer(std::string layerName)
 {
    for (int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerName == layerName)
        {
            return i;
        }
    }
 }
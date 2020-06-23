#include "CNN_Network.hpp"
using namespace std;


espresso::CNN_Network::CNN_Network(vector<espresso::layerInfo_obj*>& layerInfoArr, vector<int>& outputLayers) {

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
        else if(layerInfoArr[i]->layerType == espresso::POOLING_MAX && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new PoolingMAXLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::POOLING_AVG && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new PoolingAVGLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::PERMUTE && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new PermuteLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::FLATTEN && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new FlattenLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::DETECTION_OUTPUT && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new DetectionOutputLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::PRIOR_BOX && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new PriorBoxLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::RESHAPE && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new ReshapeLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::INNERPRODUCT && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new InnerProductLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::SOFTMAX && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new SoftMaxLayer_FPGA(layerInfoArr[i]));
        }
        else if(layerInfoArr[i]->layerType == espresso::PSROIPoolingLayer && layerInfoArr[i]->backend == espresso::FPGA_BACKEND)
        {
            m_cnn.push_back(new PSROIPoolingLayer_FPGA(layerInfoArr[i]));
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
    cout << endl;
    GetTopAndBottomLayers();
    for (int i = 0; i < m_cnn.size(); i++)
    {
        m_cnn[i]->ComputeLayerParam();
    }
}


espresso::CNN_Network::~CNN_Network()
{
    if(m_cnn[0]->m_yolo_net)
    {
        free_network(m_cnn[0]->m_yolo_net);
    }
    for(int i = 0; i < m_cnn.size(); i++)
    {
        delete m_cnn[i];
    }
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
    if(startIdx < 0 || endIdx < 0) 
    {
        for(uint32_t i = 0; i < m_cnn.size(); i++)
        {
            if(m_cnn[i]->m_layerName == start)
            {
                startIdx = i;
            }
            if(m_cnn[i]->m_layerName == end)
            {
                endIdx = i;
            }
        }
    }
    if((startIdx == -1 || endIdx == -1) || (endIdx < startIdx)) 
    {
        cout << "[ESPRESSO]: No start layer: " << start << " or end layer: " << end  << " or direction wrong" << endl;
        exit(0);
    }
}


void espresso::CNN_Network::mergeLayers(int idx, int seqID, vector<string>& sequence)
{
    m_cnn[idx]->m_fpga_merged = true;
    m_cnn[idx]->m_sequence_id = seqID;
    for(int i = 0; i < sequence.size(); i++)
    {
        int lay_idx = findLayer(sequence[i]);
        if(m_cnn[lay_idx]->m_layerType == RESIDUAL)
        {
            m_cnn[idx]->m_merged_layers.push_back(lay_idx);
            m_cnn[lay_idx]->m_fpga_merged     = true;
            m_cnn[idx]->m_fpga_do_res_layer   = true;
            m_cnn[idx]->m_residualMapDepth    = m_cnn[lay_idx]->m_bottomLayers[0]->m_blob.depth;
            m_cnn[idx]->m_numResidualMapRows  = m_cnn[lay_idx]->m_bottomLayers[0]->m_blob.numRows;
            m_cnn[idx]->m_numResidualMapsCols = m_cnn[lay_idx]->m_bottomLayers[0]->m_blob.numCols;
            m_cnn[idx]->m_residualMapData     = m_cnn[lay_idx]->m_bottomLayers[0]->m_blob.flData;
        }
        else if(m_cnn[lay_idx]->m_layerType == CONVOLUTION)
        {
            m_cnn[idx]->m_merged_layers.push_back(lay_idx);
            m_cnn[lay_idx]->m_fpga_merged     = true;
            m_cnn[idx]->m_fpga_do_kernels1x1  = true;
            m_cnn[idx]->m_num1x1Kernels       = m_cnn[lay_idx]->m_numKernels;
            m_cnn[idx]->m_kernel1x1Depth      = m_cnn[lay_idx]->m_kernelDepth;
            m_cnn[idx]->m_kernel1x1Data       = m_cnn[lay_idx]->m_flFilterData;
            m_cnn[idx]->m_bias1x1Data         = m_cnn[lay_idx]->m_flBiasData;
        }
    }
}


void espresso::CNN_Network::cfgFPGALayers(string mrgFmt_fn)
{
    ifstream infile(mrgFmt_fn);
    string line;
    while(getline(infile, line))
    {
        string sequence_str;
        stringstream check1(line);
        vector<string> sequence_tokens;
        while(getline(check1, sequence_str, ','))
        {
            sequence_tokens.push_back(sequence_str);
        }
        int seqID = stoi(sequence_tokens[0]);
        string SW_layer = sequence_tokens[2];
        vector<string> sequence(sequence_tokens.begin() + 3, sequence_tokens.end());
        int i = findLayer(SW_layer);
        if(sequence_tokens[1] == "do_1x1_res")
        {
            m_cnn[i]->m_fpga_do_1x1_res = true;
        }
        else if(sequence_tokens[1] == "do_res_1x1")
        {
            m_cnn[i]->m_fpga_do_res_1x1 = true;
        }
        else if(sequence_tokens[1] == "do_resLayer")
        {
            m_cnn[i]->m_fpga_do_res_layer = true;
        }
        else if(sequence_tokens[1] == "do_kernels1x1")
        {
            m_cnn[i]->m_fpga_do_kernels1x1 = true;
        }
        seqBgnIdxArr.push_back(i);
        mergeLayers(i, seqID, sequence);
    }
    for(int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1 && !m_cnn[i]->m_fpga_merged)
        {
            m_cnn[i]->m_kernel1x1Data           = m_cnn[i]->m_flFilterData;
            m_cnn[i]->m_bias1x1Data             = m_cnn[i]->m_flBiasData;
            m_cnn[i]->m_num1x1Kernels           = m_cnn[i]->m_numKernels;
            m_cnn[i]->m_kernel1x1Depth          = m_cnn[i]->m_kernelDepth;
            m_cnn[i]->m_fpga_do_kernels1x1      = true;
            m_cnn[i]->m_fpga_krnl_1x1_layer     = true;
        }
    }
}


void espresso::CNN_Network::Forward(string start, string end)
{
    // find start and end
    int startIdx = -1;
    int endIdx = -1;
    getBgnEndLayer(startIdx, start, endIdx, end);
    // Forward Propagation
    if(startIdx == endIdx) endIdx++;
    for(int i = startIdx; i < endIdx; i++)
    {
        if(m_cnn[i]->m_layerType == espresso::YOLO
            || m_cnn[i]->m_layerType == espresso::CONCAT
            || m_cnn[i]->m_layerType == espresso::PERMUTE     
            || m_cnn[i]->m_layerType == espresso::DETECTION_OUTPUT
            || m_cnn[i]->m_layerType == espresso::PRIOR_BOX       
            || m_cnn[i]->m_layerType == espresso::RESHAPE
            || m_cnn[i]->m_layerType == espresso::INNERPRODUCT    
            || m_cnn[i]->m_layerType == espresso::SOFTMAX                         
            || m_cnn[i]->m_layerType == espresso::UPSAMPLE        
            || m_cnn[i]->m_layerType == espresso::PSROIPoolingLayer
        ) continue;
        printLayerStats(i);
        //////
        cout << "[ESPRESSO]: Processing Layer(s)" << " " << m_cnn[i]->m_layerName;
        for(int j = 0; j < m_cnn[i]->m_merged_layers.size(); j++)
        {
            int mli = m_cnn[i]->m_merged_layers[j];
            cout << ", " << m_cnn[mli]->m_layerName;
        }
        cout << endl;
        //////
        m_cnn[i]->ComputeLayer();
        //////
        cout << "[ESPRESSO]: Finished Layer(s)" << " " << m_cnn[i]->m_layerName;
        for(int j = 0; j < m_cnn[i]->m_merged_layers.size(); j++)
        {
            int mli = m_cnn[i]->m_merged_layers[j];
            cout << ", " << m_cnn[mli]->m_layerName;
        }
        cout << endl;
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


string espresso::CNN_Network::to_string(espresso::layerType_t layerType)
{   
    switch(layerType)
    {
        case INPUT: return "INPUT";      
        case CONVOLUTION: return "CONVOLUTION"; 
        case POOLING_MAX: return "POOLING_MAX";        
        case POOLING_AVG: return "POOLING_AVG";        
        case PERMUTE: return "PERMUTE";          
        case FLATTEN: return "FLATTEN";       
        case RESIDUAL: return "RESIDUAL";          
        case DETECTION_OUTPUT: return "DETECTION_OUTPUT";    
        case PRIOR_BOX: return "PRIOR_BOX";         
        case RESHAPE: return "RESHAPE";            
        case INNERPRODUCT: return "INNERPRODUCT";      
        case SOFTMAX:  return "SOFTMAX";           
        case CONCAT: return "CONCAT";             
        case YOLO: return "YOLO";               
        case UPSAMPLE: return "UPSAMPLE";          
        case PSROIPoolingLayer: return "PSROIPoolingLayer"; 
    }
}


void espresso::CNN_Network::printLayerStats(int i)
{
    cout << "[ESPRESSO]: " << m_cnn[i]->m_layerName << endl;
    cout << "[ESPRESSO]:\tType:                 "   << to_string(m_cnn[i]->m_layerType)  << endl;
    cout << "[ESPRESSO]:\tInput Depth:          "   << m_cnn[i]->m_inputDepth            << endl;
    cout << "[ESPRESSO]:\tNum Input Rows:       "   << m_cnn[i]->m_numInputRows          << endl;
    cout << "[ESPRESSO]:\tNum Input Cols:       "   << m_cnn[i]->m_numInputCols          << endl;
    cout << "[ESPRESSO]:\tOutput Depth:         "   << m_cnn[i]->m_outputDepth           << endl;
    cout << "[ESPRESSO]:\tNum Output Rows:      "   << m_cnn[i]->m_numOutputRows         << endl;
    cout << "[ESPRESSO]:\tNum Output Cols:      "   << m_cnn[i]->m_numOutputCols         << endl;
    if(m_cnn[i]->m_layerType == espresso::CONVOLUTION)
    {
        cout << "[ESPRESSO]:\tStride:               " << m_cnn[i]->m_stride << endl;
        cout << "[ESPRESSO]:\tPadding:              " << m_cnn[i]->m_padding << endl;
        cout << "[ESPRESSO]:\tNumber of Kernels:    " << m_cnn[i]->m_numKernels << endl;
        cout << "[ESPRESSO]:\tKernel Depth:         " << m_cnn[i]->m_kernelDepth << endl;
        cout << "[ESPRESSO]:\tKernel Size:          " << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
        for(int j = 0; j < m_cnn[i]->m_merged_layers.size(); j++)
        {
            int mli = m_cnn[i]->m_merged_layers[j];
            cout << "[ESPRESSO]: " << m_cnn[mli]->m_layerName << endl;
            cout << "[ESPRESSO]:\tType:                 "   << to_string(m_cnn[mli]->m_layerType)  << endl;
            cout << "[ESPRESSO]:\tInput Depth:          "   << m_cnn[mli]->m_inputDepth            << endl;
            cout << "[ESPRESSO]:\tNum Input Rows:       "   << m_cnn[mli]->m_numInputRows          << endl;
            cout << "[ESPRESSO]:\tNum Input Cols:       "   << m_cnn[mli]->m_numInputCols          << endl;
            cout << "[ESPRESSO]:\tOutput Depth:         "   << m_cnn[mli]->m_outputDepth           << endl;
            cout << "[ESPRESSO]:\tNum Output Rows:      "   << m_cnn[mli]->m_numOutputRows         << endl;
            cout << "[ESPRESSO]:\tNum Output Cols:      "   << m_cnn[mli]->m_numOutputCols         << endl;
            if(m_cnn[mli]->m_layerType == CONVOLUTION)
            {
                cout << "[ESPRESSO]:\tStride:               " << m_cnn[mli]->m_stride << endl;
                cout << "[ESPRESSO]:\tPadding:              " << m_cnn[mli]->m_padding << endl;
                cout << "[ESPRESSO]:\tNumber of Kernels:    " << m_cnn[mli]->m_numKernels << endl;
                cout << "[ESPRESSO]:\tKernel Depth:         " << m_cnn[mli]->m_kernelDepth << endl;
                cout << "[ESPRESSO]:\tKernel Size:          " << m_cnn[mli]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << endl;
            }
        }
    }
}


void espresso::CNN_Network::setHardware(FPGA_hndl* fpga_hndl)
{
    for (int i = 0; i < m_cnn.size(); i++)
    {
        m_cnn[i]->m_fpga_hndl = fpga_hndl;
    }
}


int espresso::CNN_Network::findLayer(string layerName)
{
    for (int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerName == layerName)
        {
            return i;
        }
    }
    cout << "[ESPRESSO]: Can't find layer: " << layerName << endl;
    exit(0);
}


void espresso::CNN_Network::printMemBWStats()
{
    for(int i = 0; i < seqBgnIdxArr.size(); i++)
    {
        int sbi = seqBgnIdxArr[i];
        int optBW = ((m_cnn[sbi]->m_inputDepth
                        * m_cnn[sbi]->m_numInputRows
                        * m_cnn[sbi]->m_numInputCols)
                        + (m_cnn[sbi]->m_numKernels
                        * m_cnn[sbi]->m_kernelDepth
                        * m_cnn[sbi]->m_group
                        * m_cnn[sbi]->m_numKernelRows
                        * m_cnn[sbi]->m_numKernelCols)
        ) * PIXEL_SIZE;
        int baseBW = optBW + (m_cnn[sbi]->m_outputDepth
                                * m_cnn[sbi]->m_numOutputRows
                                * m_cnn[sbi]->m_numOutputCols
        ) * PIXEL_SIZE;
        int j_loop_end = m_cnn[sbi]->m_merged_layers.size();
        for(int j = 0; j < j_loop_end; j++)
        {
            int mli = m_cnn[sbi]->m_merged_layers[j];
            if(m_cnn[mli]->m_layerType == CONVOLUTION)
            {
                baseBW += ((m_cnn[mli]->m_inputDepth
                            * m_cnn[mli]->m_numInputRows
                            * m_cnn[mli]->m_numInputCols)
                            + (m_cnn[mli]->m_numKernels
                            * m_cnn[mli]->m_kernelDepth
                            * m_cnn[mli]->m_group
                            * m_cnn[mli]->m_numKernelRows
                            * m_cnn[mli]->m_numKernelCols)
                            + (m_cnn[mli]->m_outputDepth
                            * m_cnn[mli]->m_numOutputRows
                            * m_cnn[mli]->m_numOutputCols)
                        ) * PIXEL_SIZE;
                optBW += (m_cnn[mli]->m_numKernels
                            * m_cnn[mli]->m_kernelDepth
                            * m_cnn[mli]->m_group
                            * m_cnn[mli]->m_numKernelRows
                            * m_cnn[mli]->m_numKernelCols
                        ) * PIXEL_SIZE;
                optBW += (m_cnn[mli]->m_outputDepth
                            * m_cnn[mli]->m_numOutputRows
                            * m_cnn[mli]->m_numOutputCols
                        ) * PIXEL_SIZE;
            }
            else if(m_cnn[mli]->m_layerType == RESIDUAL)
            {
                baseBW += ((m_cnn[mli]->m_inputDepth
                            * m_cnn[mli]->m_numInputRows
                            * m_cnn[mli]->m_numInputCols)
                            + (m_cnn[mli]->m_outputDepth
                            * m_cnn[mli]->m_numOutputRows
                            * m_cnn[mli]->m_numOutputCols)
                ) * PIXEL_SIZE;
                int m_inputDepth = m_cnn[mli]->m_inputDepth / m_cnn[mli]->m_bottomLayers.size();
                optBW += (m_inputDepth
                            * (m_cnn[mli]->m_bottomLayers.size() - 1)
                            * m_cnn[mli]->m_numInputRows
                            * m_cnn[mli]->m_numInputCols
                        ) * PIXEL_SIZE;
            }
            //////
            if(j == (j_loop_end - 1))
            {
                optBW += (m_cnn[mli]->m_outputDepth
                            * m_cnn[mli]->m_numOutputRows
                            * m_cnn[mli]->m_numOutputCols
                        ) * PIXEL_SIZE;
            }
        }
        cout << "Sequence" << m_cnn[sbi]->m_sequence_id << "," << baseBW << "," << optBW << endl;
    }
}


void espresso::CNN_Network::printExecutionStats()
{
	ofstream fd;
	string WSpath = string(getenv("WORKSPACE_PATH"));
	fd.open(WSpath + "/espressoTester/build/debug/output.txt");
    for(int i = 0; i < seqBgnIdxArr.size(); i++)
    {
        int sbi = seqBgnIdxArr[i];
        fd 		<< "Sequence" 
				<< m_cnn[sbi]->m_sequence_id		<< "," 
				<< m_cnn[sbi]->m_fpga_elapsed_time	<< "," 
				<< m_cnn[sbi]->m_fpga_avgIterTime	<< "," 
				<< m_cnn[sbi]->m_fpga_memPower 		<< endl;        
    }
	fd.close();
}

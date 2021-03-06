#include "CNN_Network.hpp"
using namespace std;


espresso::CNN_Network::CNN_Network(string netName, vector<espresso::layerInfo_obj*>& layerInfoArr, vector<int>& outputLayers) 
{
    m_netName = netName;
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
    GetTopAndBottomLayers();
    for (int i = 0; i < m_cnn.size(); i++)
    {
        m_cnn[i]->ComputeLayerParam();
    }
    cfgFPGALayers();
    for (int i = 0; i < m_cnn.size(); i++)
    {
        ResidualLayer_FPGA* residuallayer_FPGA;
        ConvolutionLayer_FPGA* convolutionLayer_FPGA;
        if(m_cnn[i]->m_layerType == espresso::CONVOLUTION)
        {
            convolutionLayer_FPGA = reinterpret_cast<ConvolutionLayer_FPGA*>(m_cnn[i]);
            convolutionLayer_FPGA->makeLayerJob();
        }
        else if(m_cnn[i]->m_layerType == espresso::RESIDUAL)
        {
            residuallayer_FPGA = reinterpret_cast<ResidualLayer_FPGA*>(m_cnn[i]);
            residuallayer_FPGA->makeLayerJob();        
        }
        if(m_cnn[i]->m_layerType == espresso::CONVOLUTION || m_cnn[i]->m_layerType == espresso::RESIDUAL)
        {
            m_cnn[i]->m_layer_job->createLayerIters();
        }
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
            if(i = (sequence.size() - 1))
            {
                m_cnn[idx]->m_fpga_outDepth           = m_cnn[lay_idx]->m_outputDepth;
                m_cnn[idx]->m_fpga_numOutRows         = m_cnn[lay_idx]->m_numOutputRows;
                m_cnn[idx]->m_fpga_numOutCols         = m_cnn[lay_idx]->m_numOutputCols;
            }
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
            m_cnn[idx]->m_fpgaAct1x1          = m_cnn[lay_idx]->m_activation;
            if(i = (sequence.size() - 1))
            {
                m_cnn[idx]->m_fpga_outDepth           = m_cnn[lay_idx]->m_outputDepth;
                m_cnn[idx]->m_fpga_numOutRows         = m_cnn[lay_idx]->m_numOutputRows;
                m_cnn[idx]->m_fpga_numOutCols         = m_cnn[lay_idx]->m_numOutputCols;
            }
        }
    }
}


void espresso::CNN_Network::cfgFPGALayers()
{
    for(int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1)
        {
            m_cnn[i]->m_fpgaAct1x1              = m_cnn[i]->m_activation;
            m_cnn[i]->m_kernel1x1Data           = m_cnn[i]->m_flFilterData;
            m_cnn[i]->m_bias1x1Data             = m_cnn[i]->m_flBiasData;
            m_cnn[i]->m_num1x1Kernels           = m_cnn[i]->m_numKernels;
            m_cnn[i]->m_kernel1x1Depth          = m_cnn[i]->m_kernelDepth;
            m_cnn[i]->m_fpga_do_kernels1x1      = true;
            m_cnn[i]->m_fpga_krnl_1x1_layer     = true;
            m_cnn[i]->m_fpga_outDepth           = m_cnn[i]->m_outputDepth;
            m_cnn[i]->m_fpga_numOutRows         = m_cnn[i]->m_numOutputRows;
            m_cnn[i]->m_fpga_numOutCols         = m_cnn[i]->m_numOutputCols;
        }
        else if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 3)
        {
            m_cnn[i]->m_fpgaAct3x3              = m_cnn[i]->m_activation;
            m_cnn[i]->m_fpga_outDepth           = m_cnn[i]->m_outputDepth;
            m_cnn[i]->m_fpga_numOutRows         = m_cnn[i]->m_numOutputRows;
            m_cnn[i]->m_fpga_numOutCols         = m_cnn[i]->m_numOutputCols;            
        }
        else if(m_cnn[i]->m_layerType == RESIDUAL)
        {
            m_cnn[i]->m_fpga_do_res_layer_only   = true;
            m_cnn[i]->m_residualMapDepth        = m_cnn[i]->m_bottomLayers[0]->m_blob.depth;
            m_cnn[i]->m_numResidualMapRows      = m_cnn[i]->m_bottomLayers[0]->m_blob.numRows;
            m_cnn[i]->m_numResidualMapsCols     = m_cnn[i]->m_bottomLayers[0]->m_blob.numCols;
            m_cnn[i]->m_residualMapData         = m_cnn[i]->m_bottomLayers[1]->m_blob.flData;
            m_cnn[i]->m_fpga_outDepth           = m_cnn[i]->m_outputDepth;
            m_cnn[i]->m_fpga_numOutRows         = m_cnn[i]->m_numOutputRows;
            m_cnn[i]->m_fpga_numOutCols         = m_cnn[i]->m_numOutputCols;
        }
    }
}


void espresso::CNN_Network::cfgFPGALayers(string mrgFmt_fn)
{
    for(int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1)
        {
            m_cnn[i]->m_fpgaAct1x1              = (espresso::activation_t)0;
            m_cnn[i]->m_kernel1x1Data           = NULL;
            m_cnn[i]->m_bias1x1Data             = NULL;
            m_cnn[i]->m_num1x1Kernels           = 0;
            m_cnn[i]->m_kernel1x1Depth          = 0;
            m_cnn[i]->m_fpga_do_kernels1x1      = false;
            m_cnn[i]->m_fpga_krnl_1x1_layer     = false;
            m_cnn[i]->m_fpga_outDepth           = 0;
            m_cnn[i]->m_fpga_numOutRows         = 0;
            m_cnn[i]->m_fpga_numOutCols         = 0;
        }
        else if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 3)
        {
            m_cnn[i]->m_fpgaAct3x3              = (espresso::activation_t)0;
            m_cnn[i]->m_fpga_outDepth           = 0;
            m_cnn[i]->m_fpga_numOutRows         = 0;
            m_cnn[i]->m_fpga_numOutCols         = 0;            
        }
        else if(m_cnn[i]->m_layerType == RESIDUAL)
        {
            m_cnn[i]->m_fpga_do_res_layer_only  = false;
            m_cnn[i]->m_residualMapDepth        = 0;
            m_cnn[i]->m_numResidualMapRows      = 0;
            m_cnn[i]->m_numResidualMapsCols     = 0;
            m_cnn[i]->m_residualMapData         = 0;
            m_cnn[i]->m_fpga_outDepth           = 0;
            m_cnn[i]->m_fpga_numOutRows         = 0;
            m_cnn[i]->m_fpga_numOutCols         = 0;
        }
    }

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
            m_cnn[i]->m_fpgaAct1x1              = m_cnn[i]->m_activation;
        }
        else if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 3)
        {
            m_cnn[i]->m_fpgaAct3x3              = m_cnn[i]->m_activation;        
        }
    }
}


void espresso::CNN_Network::Forward(string start, string end)
{
    cout << endl << endl;
    // find start and end
    int startIdx = -1;
    int endIdx = -1;
    getBgnEndLayer(startIdx, start, endIdx, end);
    // Forward Propagation
    if(startIdx == endIdx) endIdx++;
#ifdef MODEL_TECH
    if(remove("/export/home/izo5011/WorkSpace/SYSC_FPGA/simulation/last_fin_layer.txt"))
    {
        perror("Error deleting file");
    }
#endif    
    for(int i = startIdx; i < endIdx; i++)
    {        
        printLayerStats(i);
        //////
        cout << "[ESPRESSO]: Processing Layer(s) " << i << " " << m_cnn[i]->m_layerName;
        for(int j = 0; j < m_cnn[i]->m_merged_layers.size(); j++)
        {
            int mli = m_cnn[i]->m_merged_layers[j];
            cout << ", " << m_cnn[mli]->m_layerName;
        }
        cout << endl << endl << endl;
        //////
        if(m_cnn[i]->m_layerType == espresso::CONVOLUTION || m_cnn[i]->m_layerType == espresso::RESIDUAL)
        {
            m_cnn[i]->ComputeLayer();
        }
        cout << "[ESPRESSO]: Finished Layer Processing " << endl;
        cout << endl << endl;
        
        // DEBUG
        // if(m_cnn[i]->m_layerType == espresso::CONVOLUTION || m_cnn[i]->m_layerType == espresso::RESIDUAL)
        // {
        //     FILE* fd = fopen(("./out_" + std::to_string(i) + ".txt").c_str(), "w");
        //     for(int a = 0; a < m_cnn[i]->m_topLayers[0]->m_blob.depth; a++)
        //     {
        //         for(int b = 0; b < m_cnn[i]->m_topLayers[0]->m_blob.numRows; b++)
        //         {
        //             for(int c = 0; c < m_cnn[i]->m_topLayers[0]->m_blob.numCols; c++)
        //             {
        //                 int idx = index3D(m_cnn[i]->m_blob.numRows, m_cnn[i]->m_blob.numCols, a, b, c);
        //                 fprintf(fd, "%f ", m_cnn[i]->m_topLayers[0]->m_blob.flData[idx]);
        //             }
        //             fprintf(fd, "\n");
        //         }
        //         fprintf(fd, "\n\n\n");                
        //     }
        //     fclose(fd);
        // }
        
        // if(i == 6)
        //     exit(0);
#ifdef MODEL_TECH    
        FILE* fd = fopen("/export/home/izo5011/WorkSpace/SYSC_FPGA/simulation/last_fin_layer.txt", "w");
        fprintf(fd, "%s\n", m_cnn[i]->m_layerName.c_str());
        fflush(fd);
        fclose(fd);
#endif
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
	
	cout << endl << endl;
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
    ofstream fd;
	string WSpath = string(getenv("WORKSPACE_PATH"));
	fd.open(WSpath + "/espressoTester/build/debug/memBWStats.csv");
    fd << ",BaseBW (B),OptBW (B)" << endl;
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
        fd << "Sequence" << m_cnn[sbi]->m_sequence_id << "," << baseBW << "," << optBW << endl;
    }
    fd.close();
}


void espresso::CNN_Network::printExecutionStats()
{
	ofstream fd;
	string WSpath = string(getenv("WORKSPACE_PATH"));
	fd.open(WSpath + "/espressoTester/build/debug/executionStats.csv");
    fd 	<< ",Total Elapsed Time (ns),Avg Iter Time (ns),Power (mW)" << endl;
    for(int i = 0; i < seqBgnIdxArr.size(); i++)
    {
        int sbi = seqBgnIdxArr[i];
        fd 		<< "Sequence" 
				<< m_cnn[sbi]->m_sequence_id		<< "," 
				<< m_cnn[sbi]->m_fpga_elapsed_time	<< "," 
				<< m_cnn[sbi]->m_fpga_avgIterTime	<< "," 
				<< m_cnn[sbi]->m_fpga_memPower 		<< endl;        
    }
}



void espresso::CNN_Network::printAccelPerfAnalyStats()
{
    ofstream fd;
	string WSpath = string(getenv("WORKSPACE_PATH"));
    string fname = WSpath + "/espressoTester/build/debug/accelPerfAnalyStats_" + m_netName + ".csv";
	fd.open(fname.c_str());
    // fd << ",calc_QUAD_TIME,sim_QUAD_time,calc_FAS_TIME,sim_FAS_TIME" << endl;
    fd << "IDX,NAME,TYPE,QUAD_TIME,FAS_TIME" << endl;
    double totalTime = 0;
    for(int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType != CONVOLUTION && m_cnn[i]->m_layerType != RESIDUAL)
        {
            continue;
        }
        fd <<  i << "," << m_cnn[i]->m_layerName << "," << to_string(m_cnn[i]->m_layerType) << ",";
        if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 3 && m_cnn[i]->m_dilation == -1) 
        {
			// fd << "Layer" << i << ",";
            // fd << m_cnn[i]->m_avg_QUAD_time0 << "," << m_cnn[i]->m_avg_QUAD_time1 << ",," << endl;
            // totalTime += m_cnn[i]->m_avg_QUAD_time0;

            fd << m_cnn[i]->m_avg_QUAD_time0 << "," << endl;           
            totalTime += m_cnn[i]->m_fpga_elapsed_time;
        }
        else if(m_cnn[i]->m_layerType == CONVOLUTION && m_cnn[i]->m_numKernelRows == 1 && m_cnn[i]->m_stride == 1)
        {
            // fd << ",," << m_cnn[i]->m_avg_FAS_time0 << "," << m_cnn[i]->m_avg_FAS_time1 << endl;
            // totalTime += m_cnn[i]->m_avg_FAS_time0;
            
            fd << "," << m_cnn[i]->m_avg_FAS_time0 << endl;
            totalTime += m_cnn[i]->m_fpga_elapsed_time;
        }
		else if(m_cnn[i]->m_layerType == RESIDUAL)
		{
            // fd << ",," << m_cnn[i]->m_avg_FAS_time0 << "," << m_cnn[i]->m_avg_FAS_time1 << endl;
            // totalTime += m_cnn[i]->m_avg_FAS_time0;
            
            fd << "," << m_cnn[i]->m_avg_FAS_time0 << endl;
            totalTime += m_cnn[i]->m_fpga_elapsed_time;            
		} 
    }
    fd << endl;
    fd << "Total Time:, "   << totalTime          << endl;
    fd << "K_3_S:, "        << K_3_S              << endl; 
    fd << "K_1_S:, "        << K_1_S              << endl; 
    fd << "MX_3X3_S:, "     << MX_3X3_S           << endl;
    fd << "MX_1X1_S:, "     << MX_1X1_S           << endl;
    fd << "NUM_QUADS:, "    << MAX_QUAD_PER_AWP   << endl;
    fd << "FPS:, "          << 1.0 / (totalTime * pow(10.0, -9.0)) << endl;
    fd << "DSPs:, "         << NUM_DSPS_PER_QUAD * K_3_S + NUM_DSPS_PER_FAS * K_1_S << " / 9024" << endl;
    fd << "BRAMs:, "        << NUM_BRAMS_PER_QUAD << " / 340" << endl;
    fd.close();
}


void espresso::CNN_Network::printNetStats()
{
    FILE *fd = fopen(m_netName.c_str(), "w");
    fprintf(fd , ",Name,Type,input channels,input dimensions,output channels,output dimensions,Kernel dimensions,Padding,Stride,group,activation\n");
    int maccCout = 0;
    int paramTot = 0;
    for(int i = 0; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType == espresso::CONVOLUTION)
        {
            string act_str = (m_cnn[i]->m_activation == espresso::LINEAR) ? "linear" : 
            (m_cnn[i]->m_activation == espresso::LEAKY) ? "leaky" : "None";
            fprintf(fd , ",%s,convolution,%d,%dx%d,%d,%dx%d,%dx%d,%d,%d,%d,%s\n",
                m_cnn[i]->m_layerName.c_str(), 
                m_cnn[i]->m_inputDepth, m_cnn[i]->m_numInputRows, m_cnn[i]->m_numInputCols,
                m_cnn[i]->m_outputDepth, m_cnn[i]->m_numOutputRows, m_cnn[i]->m_numOutputCols,
                m_cnn[i]->m_numKernelRows, m_cnn[i]->m_numKernelCols,
                m_cnn[i]->m_padding, m_cnn[i]->m_stride,m_cnn[i]->m_group, act_str.c_str()
            );
            maccCout += (m_cnn[i]->m_outputDepth * m_cnn[i]->m_numOutputRows * m_cnn[i]->m_numOutputCols);
            paramTot += (m_cnn[i]->m_outputDepth * m_cnn[i]->m_inputDepth * m_cnn[i]->m_numKernelRows * m_cnn[i]->m_numKernelCols);
            
        }
        if(m_cnn[i]->m_layerType == espresso::RESIDUAL)
        {
            fprintf(fd , ",%s,shortcut,%d,%dx%d,%d,%dx%d,-,-,-,-,-\n",
                m_cnn[i]->m_layerName.c_str(), 
                m_cnn[i]->m_inputDepth, m_cnn[i]->m_numInputRows, m_cnn[i]->m_numInputCols,
                m_cnn[i]->m_outputDepth, m_cnn[i]->m_numOutputRows, m_cnn[i]->m_numOutputCols
            );
        }
    }
    printf("MACC: %d, PARAM: %d\n", maccCout, paramTot);
    fclose(fd);
}


void espresso::CNN_Network::writeLayIt()
{
    // // FIXME: proper way is to access data structures from here and print
    // string fn = m_netName + "_partitioned_k" + std::to_string(ACCL_MAX_KRNLS) + "_d" + std::to_string(ACCL_MAX_DEPTH_SIMD) + ".csv"; 
    // FILE *fd = fopen(fn.c_str(), "w");
    // fprintf(fd , "Name,Type,input channels,input dimensions,output channels,output dimensions,Kernel dimensions,Padding,Stride,group,activation,PoolingType\n");
    // fclose(fd);
    // for(int i = 1; i < m_cnn.size(); i++)
    // {
    //     if(m_cnn[i]->m_layerType == espresso::CONVOLUTION || m_cnn[i]->m_layerType == espresso::RESIDUAL)
    //     {
    //         m_cnn[i]->m_layer_job->writeLayIt(fn, "a");
    //     }
    // }
    
    // FIXME: proper way is to access data structures from here and print
    string fn = m_netName + "_partitioned_k" + std::to_string(ACCL_MAX_KRNLS) + "_d" + std::to_string(ACCL_MAX_DEPTH_SIMD) + ".csv"; 
    FILE *fd = fopen(fn.c_str(), "w");
    fprintf(fd , "Name,Type,input channels,input dimensions,output channels,output dimensions,Kernel dimensions,Padding,Stride,group,activation,PoolingType\n");
    fclose(fd);
    for(int i = 1; i < m_cnn.size(); i++)
    {
        if(m_cnn[i]->m_layerType == espresso::CONVOLUTION || m_cnn[i]->m_layerType == espresso::RESIDUAL)
        {
            m_cnn[i]->m_layer_job->writeLayIt(fn, "a");
        }
    }
}
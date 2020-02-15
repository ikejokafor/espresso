#include "Network.hpp"


espresso::Network::Network(std::vector<espresso::layerInfo_obj> layerInfo, std::vector<int> &outputLayers) {
	
	for(int i = 0; i < layerInfo.size(); i++) 
	{
		if (layerInfo[i].layerType == espresso::INPUT) 
		{
			m_cnn.push_back(new DataLayer(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::CONCAT && layerInfo[i].backend == espresso::ESPRESSO_BACKEND) 
		{
			m_cnn.push_back(new ConcatLayer(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::CONCAT && layerInfo[i].backend == espresso::DARKNET_BACKEND) 
		{
			m_cnn.push_back(new ConcatLayer_DKN(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::CONCAT && layerInfo[i].backend == espresso::FPGA_BACKEND) 
		{
			m_cnn.push_back(new ConcatLayer_FPGA(layerInfo[i]));
		}
		else if (layerInfo[i].layerType == espresso::CONVOLUTION && layerInfo[i].backend == espresso::ESPRESSO_BACKEND) 
		{
			m_cnn.push_back(new ConvolutionLayer(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::CONVOLUTION && layerInfo[i].backend == espresso::DARKNET_BACKEND) 
		{
			m_cnn.push_back(new ConvolutionLayer_DKN(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::CONVOLUTION && layerInfo[i].backend == espresso::FPGA_BACKEND) 
		{
			m_cnn.push_back(new ConvolutionLayer_FPGA(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::UPSAMPLE && layerInfo[i].backend == espresso::ESPRESSO_BACKEND) 
		{
			m_cnn.push_back(new UpSampleLayer(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::UPSAMPLE && layerInfo[i].backend == espresso::DARKNET_BACKEND) 
		{
			m_cnn.push_back(new UpSampleLayer_DKN(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::UPSAMPLE && layerInfo[i].backend == espresso::FPGA_BACKEND) 
		{
			m_cnn.push_back(new UpSampleLayer_FPGA(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::RESIDUAL && layerInfo[i].backend == espresso::ESPRESSO_BACKEND) 
		{
			m_cnn.push_back(new ResidualLayer(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::RESIDUAL && layerInfo[i].backend == espresso::DARKNET_BACKEND) 
		{
			m_cnn.push_back(new ResidualLayer_DKN(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::RESIDUAL  && layerInfo[i].backend == espresso::FPGA_BACKEND) 
		{
			m_cnn.push_back(new ResidualLayer_FPGA(layerInfo[i]));
		}
		else if(layerInfo[i].layerType == espresso::YOLO) 
		{
			m_cnn.push_back(new YOLOLayer(layerInfo[i]));
		}		
		else
		{
            std::cout << "[ESPRESSO]: " << "Skipped Loading Layer: " << layerInfo[i].layerName << std::endl;
        }
	}
  
    for(int i = 0; i < m_cnn.size(); i++) 
    {
        std::cout << "[ESPRESSO]: Loaded Layer " << i <<  " " << m_cnn[i]->m_layerName << std::endl;
	}
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
	// get output layers
	if(outputLayers.size() != 0) 
	{
		for(int i = 0; i < outputLayers.size(); i++) 
		{
			m_outputLayers.push_back(m_cnn[outputLayers[i]]);
		}
	} 
	else 
	{
		std::stack<espresso::Layer*> outputLayers_tmp;
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



espresso::Network::~Network() 
{
	for(int i = 0; i < m_cnn.size(); i++) 
	{
		delete m_cnn[i];
	}
	free_network(m_cnn[0]->m_yolo_net);
}



void espresso::Network::Forward(std::string start, std::string end) 
{
    // find start and end
    int startIdx = -1;
    int endIdx = -1;
    if(start == " ") 
    {
        startIdx = 0;
    }
    if(end == " ") 
    {
        endIdx = m_cnn.size() - 1;
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
        std::cout << "No start layer: " << start << " or end layer: " << end  << " or direction wrong" << std::endl;
        exit(0);
    }
    for (int i = startIdx; i < (endIdx + 1); i++) 
    {
		m_cnn[i]->ComputeLayerParam();
	}
    // Forward Propagation
	for (int i = startIdx; i < (endIdx + 1); i++) 
	{
		// printLayerStats(i);
		m_cnn[i]->ComputeLayer();   
        std::cout << "Finished Layer" << " " << m_cnn[i]->m_layerName << std::endl;
		
		// int c, h, w;
		// char buf[100];
		// sprintf(buf, "layers/esp_layer_%05d.txt", i);
		// FILE *fd = fopen(buf, "w");
		// for (c = 0; c < m_cnn[i]->m_outputDepth; c++)
		// {
		// 	for (h = 0; h < m_cnn[i]->m_numOutputRows; h++)
		// 	{
		// 		for (w = 0; w < m_cnn[i]->m_numOutputCols; w++)
		// 		{
		// 			fprintf(fd, "%f ", m_cnn[i]->m_blob.flData[(c * m_cnn[i]->m_numOutputRows + h) * m_cnn[i]->m_numOutputCols + w]);
		// 		}
		// 		fprintf(fd, "\n");
		// 	}
		// 	fprintf(fd, "\n\n\n");
		// }
		// fclose(fd);
	}
}


int espresso::Network::ReturnLayerIdx(std::string name) 
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


void espresso::Network::printLayerStats(int i) 
{
	// std::cout << "Layer " << i <<  " " << m_cnn[i]->m_layerName << std::endl;
	// std::cout << "\t inputDepth:     \t\t"   << m_cnn[i]->m_inputDepth       << std::endl;
	// std::cout << "\t numInputRows:   \t\t"   << m_cnn[i]->m_numInputRows     << std::endl;
	// std::cout << "\t numInputCols:   \t\t"   << m_cnn[i]->m_numInputCols     << std::endl;
	// std::cout << "\t outputDepth:    \t\t"   << m_cnn[i]->m_outputDepth      << std::endl;
	// std::cout << "\t numOutputRows:  \t\t"   << m_cnn[i]->m_numOutputRows    << std::endl;
	// std::cout << "\t numOutputCols:  \t\t"   << m_cnn[i]->m_numOutputCols    << std::endl;       
	// if(m_cnn[i]->m_precision == espresso::FLOAT) 
	// {
	// 	std::cout << "\t precision:  \t\t\t"   << "FLOAT"                      << std::endl;
	// } 
	// else 
	// {                                                            
	// 	std::cout << "\t precision:  \t\t\t"   << "FIXED"                      << std::endl;
	// }
	// if(m_cnn[i]->m_layerType == "Convolution" || m_cnn[i]->m_layerType == "InnerProduct") 
	// {
	// 	std::cout << "\t Stride:             \t\t" << m_cnn[i]->m_stride << std::endl;
	// 	std::cout << "\t Padding:            \t\t" << m_cnn[i]->m_padding << std::endl;
	// 	std::cout << "\t Number of Kernels:  \t\t" << m_cnn[i]->m_numKernels << std::endl;
	// 	std::cout << "\t Kernel Depth:       \t\t" << m_cnn[i]->m_kernelDepth << std::endl;
	// 	std::cout << "\t Kernel Size:        \t\t" << m_cnn[i]->m_numKernelRows << "x" << m_cnn[i]->m_numKernelCols << std::endl;
	// }
	// std::cout << "\t dinFxPtLength:   \t\t"   << m_cnn[i]->m_dinFxPtLength     << std::endl;
	// std::cout << "\t dinNumFracBits:  \t\t"   << m_cnn[i]->m_dinNumFracBits    << std::endl;
	// std::cout << "\t whtFxPtLength:   \t\t"   << m_cnn[i]->m_whtFxPtLength     << std::endl;
	// std::cout << "\t whtNumFracBits:  \t\t"   << m_cnn[i]->m_whtNumFracBits    << std::endl;
	// std::cout << "\t doutFxPtLength:  \t\t"   << m_cnn[i]->m_doutFxPtLength    << std::endl;
	// std::cout << "\t doutNumFracBits: \t\t"   << m_cnn[i]->m_doutNumFracBits   << std::endl;
}

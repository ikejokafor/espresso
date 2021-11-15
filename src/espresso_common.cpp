#include "espresso_common.hpp"
using namespace std;


string espresso::to_string(espresso::layerType_t layerType)
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


string espresso::to_string(espresso::activation_t activation)
{   
    switch(activation)
    {
        case NONE: return "NONE";
        case LEAKY: return "LEAKY";
        case RELU: return "RELU";
        case LINEAR: return "LINEAR";
    }
}

#pragma once
#include <string>


namespace espresso
{
    typedef enum
    {
        NONE    = 0,
        LEAKY   = 1,
        RELU    = 2,
        LINEAR  = 3
    } activation_t;
    
    typedef enum
	{
        INPUT               = 0,
        CONVOLUTION         = 1,
        POOLING_MAX         = 2,
        POOLING_AVG         = 3,
        PERMUTE             = 4,
        FLATTEN             = 5,
        RESIDUAL            = 6,
        DETECTION_OUTPUT    = 7,
        PRIOR_BOX           = 8,
        RESHAPE             = 9,
        INNERPRODUCT        = 10,
        SOFTMAX             = 11,
        CONCAT              = 12,
        YOLO                = 13,
        UPSAMPLE            = 14,
        PSROIPoolingLayer   = 15
	} layerType_t;
    
    std::string to_string(espresso::layerType_t layerType);
    std::string to_string(espresso::activation_t activation);
}




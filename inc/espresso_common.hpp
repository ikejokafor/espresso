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
}

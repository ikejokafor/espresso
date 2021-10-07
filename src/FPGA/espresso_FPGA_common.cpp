#include "espresso_FPGA_common.hpp"


int AXI_ceil(float fth_amt, float bus_size)
{
    return (int)(ceil(fth_amt / bus_size) * bus_size);
}

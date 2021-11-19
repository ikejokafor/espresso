#include "espresso_FPGA_common.hpp"
using namespace std;


double K_3_S = 1;            // 3x3 KERNEL SIMD
double K_1_D_S = 32;         // 1x1 Kernel Depth SIMD
double K_1_S = 1;            // 1x1 Kernel SIMD
double R_S = 1;              // Row SIMD
double A_S = 32;             // Accumulation SIMD
double MX_3X3_S = 1;           // Multiply SIMD for 3x3 processing ie 32 bit compute unit can do 4 8-bit mults   
double MX_1X1_S = 1;           // Multiply SIMD for 3x3 processing ie 32 bit compute unit can do 4 8-bit mults
double CLK_PRD_NS = 12.0f;
int MAX_QUAD_PER_AWP = 8;
int QUAD_MAX_KERNELS = 64;


int AXI_sz_algn(float fth_amt, float bus_size)
{
    return (int)(ceil(fth_amt / bus_size) * bus_size);
}

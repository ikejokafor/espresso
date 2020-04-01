#define SYSTEMC
#define NUM_FAS				1
#define MAX_AWP_PER_FAS     1
#define MAX_QUAD_PER_AWP    4
#define NUM_TOTAL_QUADS     (MAX_AWP_PER_FAS * MAX_QUAD_PER_AWP)
#define QUAD_MAX_KERNELS	64
#define QUAD_MAX_DEPTH		8
#define QUAD_DPTH_SIMD		(QUAD_MAX_DEPTH * NUM_TOTAL_QUADS)

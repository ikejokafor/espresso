#define SYSTEMC
#define PIX_SEQ_CONFIG_SIZE         2
#define WINDOW_3x3_NUM_CYCLES	    5
#define MAX_NUM_INPUT_COLS          512
#define NUM_CE_PER_QUAD             8
#define NUM_FAS				        1
#define MAX_AWP_PER_FAS             1
#define MAX_QUAD_PER_AWP            4
#define NUM_TOTAL_QUADS             (MAX_AWP_PER_FAS * MAX_QUAD_PER_AWP)
#define QUAD_MAX_KERNELS	        64
#define QUAD_MAX_DEPTH		        8
#define QUAD_DPTH_SIMD		        (QUAD_MAX_DEPTH * NUM_TOTAL_QUADS)
#define PIXEL_SIZE                  2
#define CO_HIGH_WATERMARK_FACTOR    6
#define RM_LOW_WATERMARK_FACTOR     1
#define PM_LOW_WATERMARK_FACTOR     1
#define PV_LOW_WATERMARK_FACTOR     1

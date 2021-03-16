#pragma once


static int RE_HIGH_WATERMARK           = 8;
static int RE_TRAN_AMT                 = 8;
// int CM_HIGH_WATERMARK_FACTOR    = 3;
// int RM_LOW_WATERMARK_FACTOR     = 1;
// int PM_LOW_WATERMARK_FACTOR     = 1;
// int PV_LOW_WATERMARK_FACTOR     = 1;
// int RM_FETCH_FACTOR             = 1;
// int PM_FETCH_FACTOR             = 1;
// int PV_FETCH_FACTOR             = 1;
// int RE_FIFO_DEPTH               = 128;
// int CM_FIFO_DEPTH               = 512;   
// int PM_FIFO_DEPTH               = 256;
// int PV_FIFO_DEPTH               = 256;

#define WINDOW_3x3_NUM_CYCLES		5
#define AXI_BUS_SIZE				64
#define ACCL_NUM_OUTPUT             4
#define ACCL_META_OUTPUT_SIZE      (ACCL_NUM_OUTPUT * sizeof(double))
#define PIX_SEQ_CONFIG_SIZE         2
#define MAX_NUM_INPUT_COLS          512
#define NUM_CE_PER_QUAD             8
#define NUM_FAS				        1
#define MAX_AWP_PER_FAS             1
#define MAX_QUAD_PER_AWP            4
#define NUM_TOTAL_QUADS             (MAX_AWP_PER_FAS * MAX_QUAD_PER_AWP)
#define QUAD_MAX_KERNELS	        64
#define QUAD_MAX_DEPTH		        8
#define QUAD_MAX_INPUT_ROWS         ((uint64_t)608)
#define QUAD_MAX_INPUT_COLS         ((uint64_t)608)
#define QUAD_DPTH_SIMD		        (QUAD_MAX_DEPTH * NUM_TOTAL_QUADS)
#define PIXEL_SIZE                  2
#define NUM_DSPS_PER_QUAD           16
#define NUM_DSPS_PER_FAS            7 // based on Kernel Depth SIMD 32, and 5x clock
#define NUM_BRAMS_PER_QUAD          10 // based on 64 kernels
#define CO_HIGH_WATERMARK_FACTOR    3
#define RM_LOW_WATERMARK_FACTOR     1
#define PM_LOW_WATERMARK_FACTOR     1
#define PV_LOW_WATERMARK_FACTOR     1
#define RM_FETCH_FACTOR             1
#define PM_FETCH_FACTOR             1
#define PV_FETCH_FACTOR             1

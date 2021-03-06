#pragma once


#include <math.h>
#include <cmath>

extern double K_3_S;            // 3x3 KERNEL SIMD
extern double K_1_D_S;         // 1x1 Kernel Depth SIMD
extern double K_1_S;            // 1x1 Kernel SIMD
extern double R_S;              // Row SIMD
extern double A_S;             // Accumulation SIMD
extern double CLK_PRD_NS;
extern double MX_3X3_S;           // Multiply SIMD for 3x3 processing ie 32 bit compute unit can do 4 8-bit mults
extern double MX_1X1_S;          // Multiply SIMD for 3x3 processing ie 32 bit compute unit can do 4 8-bit mults

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
#define AXI_MX_BT_SZ                64

#define MAX_AWP_PER_FAS             1
extern int MAX_QUAD_PER_AWP;
extern int QUAD_MAX_KERNELS;
#define MAX_FAS_1X1_IT              4
#define MAX_FAS_1X1_IT_KRNLS        64   

#define FAS_MAX_1X1_KRNLS           1024
#define WINDOW_3x3_NUM_CYCLES		5
#define AXI_BUS_SIZE				64
#define ACCL_NUM_OUTPUT             4
#define ACCL_META_OUTPUT_SIZE       (ACCL_NUM_OUTPUT * sizeof(double))
#define PIX_SEQ_CONFIG_SIZE         2
#define MAX_NUM_INPUT_COLS          512
#define NUM_CE_PER_QUAD             8
#define NUM_FAS				        1
#define NUM_TOTAL_QUADS             (MAX_AWP_PER_FAS * MAX_QUAD_PER_AWP)
#define QUAD_MAX_DEPTH		        8
#define MAX_INPUT_ROWS              ((uint64_t)608)
#define MAX_INPUT_COLS              ((uint64_t)608)
#define ACCL_MAX_DEPTH_SIMD		    (QUAD_MAX_DEPTH * NUM_TOTAL_QUADS)
#define ACCL_MAX_KRNLS              (MAX_AWP_PER_FAS * QUAD_MAX_KERNELS)
#define MAX_KRNL_SIZE               3
#define PIXEL_SIZE                  2
#define NUM_DSPS_PER_QUAD           16
#define NUM_DSPS_PER_FAS            7 // based on Kernel Depth SIMD 32, and 5x clock
#define NUM_BRAMS_PER_QUAD          10 // based on 64 kernels
#define CO_HIGH_WATERMARK_FACTOR    3
#define RM_LOW_WATERMARK_FACTOR     1
#define PM_LOW_WATERMARK_FACTOR     1
#define PV_LOW_WATERMARK_FACTOR     1
#define RM_FETCH_FACTOR             10
#define PM_FETCH_FACTOR             5
#define PV_FETCH_FACTOR             10
#define OB_STORE_FACTOR             10
#define SYSC_MAX_KRNL_DEPTH         max(QUAD_MAX_DEPTH, QUAD_MAX_KERNELS)


int AXI_sz_algn(float fth_amt, float bus_size);

#pragma once


#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <thread>
#include <algorithm>
#include <functional>
#include "espresso_FPGA_common.hpp"
#include "SYSC_FPGA_shim.hpp"
#include "Layer_Iteration.hpp"
#include "fixedPoint.hpp"
#include "InputMaps.hpp"
#include "Kernels.hpp"
#include "KernelBias.hpp"
#include "ResidualMaps.hpp"
#include "OutputMaps.hpp"
#include "PartialMaps.hpp"
#include "Prev1x1Maps.hpp"



static double K_3_S = 1;
static double K_1_D_S = 32;
static double K_1_S = 1;
static double R_S = 1;
static double A_S = 32;
static double CLK_PRD_NS = 10.0f;


typedef struct
{
	InputMaps* inputMaps;
	Kernels* kernels3x3;
	Kernels* kernels1x1;
	KernelBias* kernels3x3Bias;
	KernelBias* kernels1x1Bias;
	PartialMaps* partialMaps;
    Prev1x1Maps* prev1x1maps;
	ResidualMaps* residualMaps;
	OutputMaps* outputMaps;
    opcode_t opcode;
} layAclPrm_t;

#ifdef SYSTEMC
class StatPayload : public Accel_Payload
{
    public:
        StatPayload() { m_buffer = NULL; m_size = 0; m_remAddress = -1; }
        ~StatPayload() { }
		void serialize() { }
        void deserialize() { }
};

class DummyPayload : public Accel_Payload
{
    public:
        DummyPayload() { m_buffer = NULL; m_size = 0; m_remAddress = -1; }
        ~DummyPayload() { }
		void serialize() { }
        void deserialize() { }
};
#endif

class Layer_Job
{
    public:
        Layer_Job(
			std::string layerName,
			int inputMapDepth,
			int numInputMapRows,
			int numInputMapCols,
			float* inputMapData,
			int num3x3Kernels,
			int kernelDepth,
			int numKernelRows,
			int numKernelCols,
			float* kernel3x3Data,
			int outputMapDepth,
			int numOutputMapRows,
			int numOutputMapCols,
			int residualMapDepth,
			int numResidualMapRows,
			int numResidualMapCols,
			float* residualMapData,
			int num1x1Kernels,
			int kernel1x1Depth,
			float* kernel1x1Data,
			float* kernel3x3Bias,
			float* kernel1x1Bias,
			int stride,
			bool upsample,
			int padding,
			bool do_resLayer,
            bool do_resLayer_only,
            bool m_fpgaAct3x3,
            bool m_fpgaAct1x1,
			bool do_kernels1x1,
			FPGA_hndl* fpga_hndl,
			bool krnl_1x1_layer,
            bool do_1x1_res,
            bool do_res_1x1,
			int fxPtLength = 16,
			int numFracBits = 14
	    );
        ~Layer_Job();
        void createLayerIters();
		layAclPrm_t* createAccelParams(
            int krnl_iter,
            int dpth_iter,
            int depthBgn,
            int depth,
            int krnl3x3Bgn,
            int numKrnl3x3,
			bool& del_res,
			bool& del_1x1			
		);
		void printConfig(Layer_Iteration* lay_it);
        void process(double& elapsed_time, double& avgIterTime, double& memPower, double& avg_QUAD_time0, double& avg_FAS_time0, double& avg_QUAD_time1, double& avg_FAS_time1);
        void calcAccelPerfAnalyStats(Layer_Iteration* lay_it, double& avg_QUAD_time, double& avg_FAS_time);


        void process(float* layOut);
        void esp_copy(int desDepth, int nDesRows, int nDesCols, float* src, float* dst);
        void UpSample(int inputDepth, int numInputRows, int numInputCols, int stride, float* inMap, float* outMap);
        void do_conv(
            int num_input_rows, int num_input_cols, float* inMap, 
            int stride, int padding, bool doAct,
            int nKR, int nKC, int kernelDepth, 
            int num_kernels, float* filters, float* bias, bool doBias,
            int num_output_rows, int num_output_cols, float* outMap
        );

        void do_accum(int num_accum_rows, int num_accum_cols, int accum_depth, float* inMapA, float* inMapB, float* outMap);

		std::string m_layerName;
        int m_inputMapDepth;
        int m_numInputMapRows;
        int m_numInputMapCols;
		int m_outputMapDepth;
		int m_numOutputMapRows;
		int m_numOutputMapCols;
        int m_num3x3Kernels;
		int m_num1x1Kernels;
		int m_kernel1x1Depth;
        int m_kernelDepth;
        int m_numKernelRows;
        int m_numKernelCols;
	    int m_fxPtLength;
        int m_numFracBits;
		int m_stride;
		bool m_upsample;
		int m_padding;
		bool m_do_resLayer;
        bool m_do_resLayer_only;
		bool m_do_kernels1x1;
        bool m_act3x3;
        bool m_act1x1;
		bool m_krnl1x1_pding;
		int m_krnl1x1_pad_bgn;
		int m_krnl1x1_pad_end;
	    InputMaps* m_inputMaps;
	    Kernels* m_kernels3x3;
		Kernels* m_kernels1x1;
		KernelBias* m_kernels3x3Bias;
		KernelBias* m_kernels1x1Bias;
		ResidualMaps* m_residualMaps;
		OutputMaps*	m_outputMaps;
		std::vector<std::vector<Layer_Iteration*>> m_lay_it_arr;
		int m_num_depth_iter;
		int m_num_krnl_iter;
		bool m_krnl_1x1_layer;
        bool m_do_1x1_res;
        bool m_do_res_1x1;
		FPGA_hndl* m_fpga_hndl;
#ifdef SYSTEMC
		DummyPayload* m_Dpyld;
		StatPayload* m_Spyld;
		SYSC_FPGA_hndl* m_sysc_fpga_hndl;
        int m_pseudo_addr;
#else

#endif
};
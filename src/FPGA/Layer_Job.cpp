#include "Layer_Job.hpp"
using namespace std;


bool isPowerOfTwo(ulong x)
{
    return ((x & (x - 1)) == 0);
}


Layer_Job::Layer_Job(
    string layerName,
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
    bool fpgaAct3x3,
    bool fpgaAct1x1,
    bool do_kernels1x1,
    FPGA_hndl* fpga_hndl,
    bool krnl_1x1_layer,
    bool do_1x1_res,
    bool do_res_1x1,
    int fxPtLength,
    int numFracBits
) {
    m_inputMaps             = NULL;
    m_kernels3x3            = NULL;
    m_residualMaps          = NULL;
    m_outputMaps            = NULL;
    m_kernels3x3Bias        = NULL;
    m_kernels1x1            = NULL;
    m_kernels1x1Bias        = NULL;
	m_Spyld					= NULL;
	m_Dpyld					= NULL;
    m_layerName             = layerName         ;
    m_inputMapDepth         = inputMapDepth     ;
    m_numInputMapRows       = numInputMapRows   ;
    m_numInputMapCols       = numInputMapCols   ;
    m_outputMapDepth        = outputMapDepth    ;
    m_numOutputMapRows      = numOutputMapRows  ;
    m_numOutputMapCols      = numOutputMapCols  ;
    m_num3x3Kernels         = num3x3Kernels     ;
    m_kernelDepth           = kernelDepth       ;
    m_numKernelRows         = numKernelRows     ;
    m_numKernelCols         = numKernelCols     ;
    m_num1x1Kernels         = num1x1Kernels     ;
    m_kernel1x1Depth        = kernel1x1Depth    ;
    m_fxPtLength            = fxPtLength        ;
    m_numFracBits           = numFracBits       ;
    m_stride                = stride            ;
    m_upsample              = upsample          ;
    m_padding               = padding           ;
    m_do_resLayer           = do_resLayer       ;
    m_do_resLayer_only      = do_resLayer_only  ;
    m_act3x3                = fpgaAct3x3        ;
    m_act1x1                = fpgaAct1x1        ;
    m_do_kernels1x1         = do_kernels1x1     ;
    m_krnl1x1_pding         = false             ;
    m_do_1x1_res            = do_1x1_res        ;
    m_do_res_1x1            = do_res_1x1        ;
    m_inputMaps             = new InputMaps(fpga_hndl, inputMapDepth, numInputMapRows, numInputMapCols, inputMapData);
    m_kernels3x3            = new Kernels(fpga_hndl, m_num3x3Kernels, kernelDepth, numKernelRows, numKernelCols, kernel3x3Data);
    if(do_resLayer || m_do_resLayer_only)
    {
        m_residualMaps      = new ResidualMaps(fpga_hndl, residualMapDepth, numResidualMapRows, numResidualMapCols, residualMapData);
    }
    m_kernels3x3Bias        = new KernelBias(fpga_hndl, num3x3Kernels, kernel3x3Bias);
    m_krnl1x1_pad_bgn = -1;
    m_krnl1x1_pad_end = -1;
    if(do_kernels1x1)
    {
        m_krnl1x1_pad_bgn = num1x1Kernels;
        if(!isPowerOfTwo(num1x1Kernels))
        {
            m_krnl1x1_pding = true;
            m_krnl1x1_pad_end = pow((float)2, (int)ceil(log2(num1x1Kernels)));
        }
        else
        {
            m_krnl1x1_pad_end = num1x1Kernels;
        }
        m_kernels1x1        = new Kernels(fpga_hndl, num1x1Kernels, m_kernel1x1Depth, 1, 1, kernel1x1Data);
        m_kernels1x1Bias    = new KernelBias(fpga_hndl, num1x1Kernels, kernel1x1Bias);
    }
    m_krnl_1x1_layer = krnl_1x1_layer;
	m_fpga_hndl		 = fpga_hndl;
#ifdef SYSTEMC
    m_sysc_fpga_hndl    = reinterpret_cast<SYSC_FPGA_hndl*>(fpga_hndl);
    m_Spyld = new StatPayload();
	m_Spyld->m_size = ACCL_META_OUTPUT_SIZE;
    m_Spyld->m_buffer = (void*)malloc(m_Spyld->m_size);	
#else

#endif
}


Layer_Job::~Layer_Job()
{
    (m_inputMaps) ? delete m_inputMaps : void();
	(m_kernels3x3) ? delete m_kernels3x3 : void();
	(m_residualMaps) ? delete m_residualMaps : void();
	(m_outputMaps) ? delete m_outputMaps : void();
	(m_kernels3x3Bias) ? delete m_kernels3x3Bias : void();
    (m_kernels1x1) ? delete m_kernels1x1 : void();
    (m_kernels1x1Bias) ? delete m_kernels1x1Bias : void();
	(m_Spyld) ? delete m_Spyld : void();
	(m_Dpyld) ? delete m_Dpyld : void();
    m_inputMaps = NULL;
	m_kernels3x3 = NULL;
	m_residualMaps = NULL;
	m_outputMaps = NULL;
	m_kernels3x3Bias = NULL;
    m_kernels1x1 = NULL;
    m_kernels1x1Bias = NULL;
	m_Spyld = NULL;
	m_Dpyld = NULL;

    int i_end = m_lay_it_arr.size();
    for(int i = 0; i < i_end; i++)
    {
        int j_end = m_lay_it_arr[i].size();
        for(int j = 0; j < j_end; j++)
        {
            delete m_lay_it_arr[i][j];
        }
    }
}


void Layer_Job::createLayerIters()
{
    m_num_krnl_iter = (m_krnl_1x1_layer || m_do_resLayer_only) ? 1 : ceil((double)m_num3x3Kernels / (double)(NUM_TOTAL_QUADS * QUAD_MAX_KERNELS));
    m_num_depth_iter = (m_krnl_1x1_layer|| m_do_resLayer_only) ? 1 : ceil((double)m_inputMapDepth / (double)(NUM_TOTAL_QUADS * QUAD_DPTH_SIMD));
    int remNumKrnl = m_num3x3Kernels;
    int numKrnl;
    layAclPrm_t* layAclPrm;
    m_lay_it_arr.resize(m_num_krnl_iter);
    for(int i = 0, krnlBgn = 0; i < m_num_krnl_iter; i++, krnlBgn += numKrnl)
    {
        numKrnl = min(remNumKrnl, QUAD_MAX_KERNELS);
        int remDepth = m_inputMapDepth;
        int depth;
        for(int j = 0, depthBgn = 0; j < m_num_depth_iter; j++, depthBgn += depth)
        {
			bool del_res;					
			bool del_1x1;
            depth = min(QUAD_DPTH_SIMD, remDepth);
            layAclPrm = createAccelParams(
                i,
                j,
                depthBgn,
                depth,
                krnlBgn,
                numKrnl,
				del_res,
				del_1x1	
            );
            m_lay_it_arr[i].push_back(new Layer_Iteration(
				m_fpga_hndl,
                layAclPrm->opcode,
                layAclPrm->inputMaps,
                layAclPrm->kernels3x3,
                layAclPrm->kernels1x1,
                layAclPrm->kernels3x3Bias,
                layAclPrm->kernels1x1Bias,
                layAclPrm->partialMaps,
                layAclPrm->residualMaps,
                layAclPrm->outputMaps,
                layAclPrm->prev1x1maps,
                m_stride,
                m_upsample,
                m_padding,
                m_act3x3,
                m_act1x1,
                m_krnl1x1_pding,
                m_krnl1x1_pad_bgn,
                m_krnl1x1_pad_end,
				del_res,
				del_1x1	
            ));
            remDepth -= depth;
        }
        remNumKrnl -= numKrnl;
    }
}


layAclPrm_t* Layer_Job::createAccelParams(
    int krnl_iter,
    int dpth_iter,
    int depthBgn,
    int depth,
    int krnl3x3Bgn,
    int numKrnl3x3,
	bool& del_res,
	bool& del_1x1)
{
    layAclPrm_t* layAclPrm = new layAclPrm_t;
    memset(layAclPrm, 0x0, sizeof(layAclPrm_t));
    layAclPrm->inputMaps = (m_krnl_1x1_layer || m_do_resLayer_only) ? NULL : m_inputMaps->GetVolume(depthBgn, depth);
    layAclPrm->kernels3x3 = (m_krnl_1x1_layer || m_do_resLayer_only) ? NULL : m_kernels3x3->GetVolume(krnl3x3Bgn, numKrnl3x3, depthBgn, depth);
    layAclPrm->kernels3x3Bias = (m_krnl_1x1_layer || m_do_resLayer_only) ? NULL : m_kernels3x3Bias->GetVolume(krnl3x3Bgn, numKrnl3x3);
    layAclPrm->opcode = (opcode_t)-1;
    bool first_depth_iter = (dpth_iter == 0);
    bool last_depth_iter = (dpth_iter == (m_num_depth_iter - 1));
    bool first_krnl_iter = (krnl_iter == 0);
    //////
    if(m_do_1x1_res && last_depth_iter && m_num_depth_iter > 1 && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_0;
    }
    else if(m_do_1x1_res && last_depth_iter && m_num_depth_iter > 1 && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_1;
    }
    else if(m_do_1x1_res && last_depth_iter && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_2;
    }
    else if(m_do_1x1_res && last_depth_iter && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_3;
    }
    else if(m_do_res_1x1 && last_depth_iter && m_num_depth_iter > 1 && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_4;
    }
    else if(m_do_res_1x1 && last_depth_iter && m_num_depth_iter > 1 && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_5;
    }
    else if(m_do_res_1x1 && last_depth_iter && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_6;
    }
    else if(m_do_res_1x1 && last_depth_iter && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_7;
    }
    else if(m_do_resLayer && last_depth_iter && m_num_depth_iter > 1)
    {
        layAclPrm->opcode = OPCODE_8;
    }
    else if(m_do_resLayer && last_depth_iter && m_num_depth_iter == 1)
    {
        layAclPrm->opcode = OPCODE_9;
    }
    else if(m_do_kernels1x1 && !m_krnl_1x1_layer && last_depth_iter && m_num_depth_iter > 1 && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_10;
    }
    else if(m_do_kernels1x1 && !m_krnl_1x1_layer && last_depth_iter && m_num_depth_iter > 1 && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_11;
    }
    else if(m_do_kernels1x1 && !m_krnl_1x1_layer && last_depth_iter && first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_12;
    }
    else if(m_do_kernels1x1 && !m_krnl_1x1_layer && last_depth_iter && !first_krnl_iter)
    {
        layAclPrm->opcode = OPCODE_13;
    }
    else if(m_krnl_1x1_layer)
    {
        layAclPrm->opcode = OPCODE_14;
    }
    else if(!first_depth_iter)
    {
        layAclPrm->opcode = OPCODE_15;
    }
    else if(m_do_resLayer_only)
    {
        layAclPrm->opcode = OPCODE_17;
    }
    else
    {
        layAclPrm->opcode = OPCODE_16;
    }
    //////
    if(last_depth_iter && m_do_resLayer && !m_do_1x1_res)
    {
        layAclPrm->residualMaps = m_residualMaps->GetVolume(krnl3x3Bgn, numKrnl3x3);
		del_res = true;
    }
    else if((last_depth_iter && m_do_resLayer && m_do_1x1_res && first_krnl_iter) || m_do_resLayer_only)
    {
        layAclPrm->residualMaps = m_residualMaps;
		del_res = false;
    }
    //////
    if(m_do_kernels1x1 && last_depth_iter)
    {
        layAclPrm->kernels1x1 = (m_krnl_1x1_layer) ? m_kernels1x1 : m_kernels1x1->GetVolume(0, m_kernels1x1->m_numKernels, krnl3x3Bgn, numKrnl3x3);
		del_1x1 = (m_krnl_1x1_layer) ? false : true;
        layAclPrm->kernels1x1Bias = m_kernels1x1Bias;
        if(!m_krnl1x1_pding)
        {
            layAclPrm->outputMaps = new OutputMaps(m_fpga_hndl, m_kernels1x1->m_numKernels, m_numOutputMapRows, m_numOutputMapCols);
        }
        else
        {
            layAclPrm->outputMaps = new OutputMaps(m_fpga_hndl, m_kernels1x1->m_numKernels + (m_krnl1x1_pad_end - m_krnl1x1_pad_bgn), m_numOutputMapRows, m_numOutputMapCols);
        }
    }
    else
    {
        layAclPrm->outputMaps = new OutputMaps(m_fpga_hndl, numKrnl3x3, m_numOutputMapRows, m_numOutputMapCols);
    }
    //////
    if(m_do_resLayer_only)
    {
        layAclPrm->outputMaps = new OutputMaps(m_fpga_hndl, m_residualMaps->m_residualMapDepth, m_residualMaps->m_numResidualMapRows, m_residualMaps->m_numResidualMapCols);
    }
    //////
    if((m_do_1x1_res || m_do_res_1x1 || m_do_kernels1x1) && !m_krnl_1x1_layer && last_depth_iter && !first_krnl_iter)
    {
        layAclPrm->prev1x1maps = new Prev1x1Maps(m_fpga_hndl, m_lay_it_arr[krnl_iter - 1][m_num_depth_iter - 1]->m_outputMaps);
    }
    //////
    if(m_krnl_1x1_layer)
    {
        layAclPrm->partialMaps = new PartialMaps(m_fpga_hndl, m_inputMaps);
    }
    else if(m_do_resLayer_only)
    {
        // See ResidualLayer.cpp:ComputeLayerParam()
        layAclPrm->partialMaps = new PartialMaps(m_fpga_hndl, m_inputMaps->m_inputMapDepth / 2, m_inputMaps->m_numInputMapRows, m_inputMaps->m_numInputMapCols, m_inputMaps->m_cpu_data);
    }
    else if(first_depth_iter)
    {
        layAclPrm->partialMaps = NULL;
    }
    else
    {
        layAclPrm->partialMaps = new PartialMaps(m_fpga_hndl, m_lay_it_arr[krnl_iter][dpth_iter - 1]->m_outputMaps);
    }
    return layAclPrm;
}


void Layer_Job::process(double& elapsed_time, double& avgIterTime, double& memPower, double& avg_QUAD_time0, double& avg_FAS_time0, double& avg_QUAD_time1, double& avg_FAS_time1)
{
    cout << "[ESPRESSO]: " << m_num_krnl_iter << " Kernel Iteration(s)" << endl;
    cout << "[ESPRESSO]: " << m_num_depth_iter << " Depth Iterations(s)" << endl;
	cout << endl << endl;
	m_Dpyld = new DummyPayload();

    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            printConfig(m_lay_it_arr[k][d]);
            if(k == 0 && d == 0)
            {
                calcAccelPerfAnalyStats(m_lay_it_arr[k][d], avg_QUAD_time0, avg_FAS_time0);
            }
			cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tProcessing Kernel Iteration - " << (k + 1) << "/" << m_num_krnl_iter << endl;
            cout << "[ESPRESSO]:\tProcessing Depth Iteration - " << (d + 1)  << "/" << m_num_depth_iter << endl;
			cout << endl << endl;
            m_sysc_fpga_hndl->wrConfig(m_lay_it_arr[k][d]->m_accelCfg);
            (m_lay_it_arr[k][d]->m_inputMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_inputMaps)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_kernels3x3) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels3x3)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_kernels3x3Bias) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels3x3Bias)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_kernels1x1) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels1x1)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_kernels1x1Bias) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels1x1Bias)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_partialMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_partialMaps)
				: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_residualMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_residualMaps)
                : m_sysc_fpga_hndl->wrParam(m_Dpyld);
            (m_lay_it_arr[k][d]->m_prev1x1Maps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_prev1x1Maps)
                : m_sysc_fpga_hndl->wrParam(m_Dpyld);
            m_sysc_fpga_hndl->sendStart();
            m_sysc_fpga_hndl->waitComplete();
            m_sysc_fpga_hndl->getOutput(m_lay_it_arr[k][d]->m_outputMaps);
			m_sysc_fpga_hndl->getOutput(m_Spyld);
            double* ptr = (double*)m_Spyld->m_buffer;
            elapsed_time += (ptr[0]);
            memPower += (ptr[1]);
			if(k == 0 && d == 0)
			{
				avg_QUAD_time1 = (ptr[2]);
				avg_FAS_time1 = (ptr[3]);
			}
            cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tFinished Kernel Iteration - " << (k + 1) << endl;
            cout << "[ESPRESSO]:\tFinished Depth Iteration - "  << (d + 1) << endl;
            cout << "[ESPRESSO]: QUAD time - " << ptr[2] << endl;
            cout << "[ESPRESSO]: FAS time - " << ptr[3] << endl;
			cout << endl << endl;
        }
    }
	double numTotalIter = m_num_krnl_iter * m_num_depth_iter;
	avgIterTime = elapsed_time / numTotalIter;
	cout << "[ESPRESSO]: Total Layer Processing Time - " << elapsed_time << " ns " << endl;
    cout << "[ESPRESSO]: Avgerage Layer Iteration Time - " << avgIterTime << " ns " << endl;
    cout << "[ESPRESSO]: Total Power Consumed - " << memPower << " mW " << endl;
	cout << endl << endl;
	delete(m_Dpyld);
}


void Layer_Job::process(float* layOut)
{
    cout << "[ESPRESSO]: " << m_num_krnl_iter << " Kernel Iteration(s)" << endl;
    cout << "[ESPRESSO]: " << m_num_depth_iter << " Depth Iterations(s)" << endl;
	cout << endl << endl;

    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            int stride = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_stride;
            int upsample = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_upsample;
            int padding = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_padding;
            bool act3x3 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_act3x3;
            bool act1x1 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_act1x1;
            opcode_t opcode = m_lay_it_arr[k][d]->m_opcode;
            InputMaps* inMaps = m_lay_it_arr[k][d]->m_inputMaps;
            Kernels* kernels3x3 = m_lay_it_arr[k][d]->m_kernels3x3;
            KernelBias* kernels3x3Bias = m_lay_it_arr[k][d]->m_kernels3x3Bias;
            Kernels* kernels1x1 = m_lay_it_arr[k][d]->m_kernels1x1;
            KernelBias* kernels1x1Bias = m_lay_it_arr[k][d]->m_kernels1x1Bias;
       		PartialMaps* partMaps = m_lay_it_arr[k][d]->m_partialMaps;
            ResidualMaps* resdMaps = m_lay_it_arr[k][d]->m_residualMaps;
            Prev1x1Maps* prev1x1Maps = m_lay_it_arr[k][d]->m_prev1x1Maps;
            OutputMaps* outMaps = m_lay_it_arr[k][d]->m_outputMaps;
            int depth = max(1024, QUAD_DPTH_SIMD); // FIXME, hardcoding
            float* intmStrgA = new float[QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * depth];
            float* intmStrgB = new float[QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * depth];
            int qd_nOutRows = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_num_output_rows;
            int qd_nOutCols = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_num_output_cols;
            int qd_outDpth = (kernels3x3) ? kernels3x3->m_numKernels : -1;
            if(upsample)
            {
                UpSample(inMaps->m_inputMapDepth, inMaps->m_numInputMapRows, inMaps->m_numInputMapCols, stride, (float*)inMaps->m_buffer, intmStrgA);
            }
            else
            {
                esp_copy(
                    (float*)inMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    inMaps->m_inputMapDepth
                );
            }
            
            FILE *fd = fopen("./intmStrgA.txt", "w");
            for(int d = 0; d < inMaps->m_inputMapDepth; d++)
            {
                for(int r = 0; r < inMaps->m_numInputMapRows; r++)
                {
                    for(int c = 0; c < inMaps->m_numInputMapCols; c++)
                    {
                        int idx = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, d, r, c);
                        fprintf(fd, "%f ", intmStrgA[idx]);
                    }
                    fprintf(fd, "\n");
                }
                fprintf(fd, "\n\n\n");
            }
            fclose(fd);
            
  
            if(opcode == OPCODE_0)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                do_accum( // resd accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)resdMaps->m_buffer,
                    intmStrgA
                );               
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_1)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                do_accum( // prev1x1 Maps accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_2)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)resdMaps->m_buffer,
                    intmStrgB
                );
                esp_copy(
                    intmStrgB,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_3)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                do_accum( // prev1x1 Map accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgB
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_4)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)resdMaps->m_buffer,
                    intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_5)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)resdMaps->m_buffer,
                    intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgB
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_6)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)resdMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_7)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)resdMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                do_accum( // prev1x1Map accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgA
                );                
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_8)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)resdMaps->m_buffer,
                    intmStrgB
                );               
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_9)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // resdMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)resdMaps->m_buffer,
                    intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_10)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_11)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMap accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );             
                do_accum( // prev1x1Map accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_12)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_13)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, intmStrgB,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, false,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgA,
                    (float*)prev1x1Maps->m_buffer,
                    intmStrgB
                ); 
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_14)
            {
                do_conv( // 1x1 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    1, 0, act1x1,
                    1, 1, kernels1x1->m_kernelDepth,
                    kernels1x1->m_numKernels, (float*)kernels1x1->m_buffer, (float*)kernels1x1Bias->m_buffer, true,
                    outMaps->m_numOutputMapRows, outMaps->m_numOutputMapCols, intmStrgB
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_15)
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, false,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );
                do_accum( // partMaps accum
                    qd_nOutRows,
                    qd_nOutCols,
                    qd_outDpth,
                    intmStrgB,
                    (float*)partMaps->m_buffer,
                    intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_16) 
            {
                do_conv( // 3x3 conv
                    m_numInputMapRows, m_numInputMapCols, intmStrgA,
                    stride, padding, act3x3,
                    3, 3, kernels3x3->m_kernelDepth,
                    kernels3x3->m_numKernels, (float*)kernels3x3->m_buffer, (float*)kernels3x3Bias->m_buffer, true,
                    qd_nOutRows, qd_nOutCols, intmStrgB
                );                
                esp_copy(
                    intmStrgB,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_17)
            {
                do_accum( // resdMaps accum
                    inMaps->m_numInputMapRows,
                    inMaps->m_numInputMapCols,
                    inMaps->m_inputMapDepth,
                    (float*)inMaps->m_buffer,
                    (float*)resdMaps->m_buffer,
                    intmStrgA
                );
                esp_copy(
                    intmStrgA,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    (float*)outMaps->m_buffer,
                    QUAD_MAX_INPUT_ROWS, 
                    QUAD_MAX_INPUT_COLS,
                    outMaps->m_outputMapDepth
                );
                free(intmStrgA);
                free(intmStrgB);
            }            
        }
    }
    int k_end = m_lay_it_arr.size() - 1;
    int d_end = m_lay_it_arr[k_end].size() - 1;
    layOut = (float*)m_lay_it_arr[k_end][d_end]->m_outputMaps->m_buffer;  
    int outputMapDepth = m_lay_it_arr[k_end][d_end]->m_outputMaps->m_outputMapDepth;
    int numOutputMapRows = m_lay_it_arr[k_end][d_end]->m_outputMaps->m_numOutputMapRows;
    int numOutputMapCols = m_lay_it_arr[k_end][d_end]->m_outputMaps->m_numOutputMapCols;
    esp_copy(
        (float*)m_lay_it_arr[k_end][d_end]->m_outputMaps->m_buffer,
        QUAD_MAX_INPUT_ROWS, 
        QUAD_MAX_INPUT_COLS,
        layOut,
        numOutputMapRows, 
        numOutputMapCols,
        outputMapDepth
    );
    

    // Debug ----------------------------------------------------------
    FILE *fd = fopen((m_layerName + ".txt").c_str(), "w");
    for(int d = 0; d < outputMapDepth; d++)
    {
        for(int r = 0; r < numOutputMapRows; r++)
        {
            for(int c = 0; c < numOutputMapCols; c++)
            {
                int idx = index3D(numOutputMapRows, numOutputMapCols, d, r, c);
                fprintf(fd, "%f ", layOut[idx]);
            }
            fprintf(fd, "\n");
        }
        fprintf(fd, "\n\n\n");
    }
    fclose(fd);
    exit(0);
    // Debug ----------------------------------------------------------
}


void Layer_Job::esp_copy(float* src, int nSRows, int nSCols, float* dst, int nDRows, int nDCols, int dDepth)
{
    for(int k = 0; k < dDepth; k++)
    {
        for(int i = 0; i < nDRows; i++)
        {
            for(int j = 0; j < nDCols; j++)
            {
                int sidx = index3D(nSRows, nSCols, k, i, j);
                int didx = index3D(nDRows, nDCols, k, i, j);
                dst[didx] = src[sidx];
            }
        }
    }
}


void Layer_Job::UpSample(int inputDepth, int numInputRows, int numInputCols, int stride, float* inMaps, float* outMap)
{
	for(int k = 0; k < inputDepth; ++k)
    {
		for(int j = 0; j < numInputRows * stride; ++j)
        {
			for(int i = 0; i < numInputCols * stride; ++i)
            {
				int in_index = k * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_ROWS + (j / stride) * QUAD_MAX_INPUT_ROWS + i / stride;
				int out_index = k * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_ROWS * stride * stride + j * QUAD_MAX_INPUT_ROWS * stride + i;
				outMap[out_index] = inMaps[in_index];
			}
		}
	}
}


void Layer_Job::do_conv(
    int num_input_rows, int num_input_cols, float* inMaps, 
    int stride, int padding, bool doAct,
    int nKR, int nKC, int kernelDepth, 
    int num_kernels, float* filters, float* bias, bool doBias,
    int num_output_rows, int num_output_cols, float* outMap)
{   
    int nthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nthreads);
    for (int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
        {
            for (int m = bi_m; m < ei_m; m++) 
            {
                for (int x = 0, a = 0; x < num_output_rows; x++, a += stride)
                {
                    for (int y = 0, b = 0; y < num_output_cols; y++, b += stride)
                    {
                        int do_i = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, m, x, y);
                        if(doBias) outMap[do_i] = bias[m];
                        for (int k = 0; k < kernelDepth; k++)
                        {
                            for (int i = a - padding, kr = 0; kr < nKR; i++, kr++)
                            {
                                for (int j = b - padding, kc = 0; kc < nKC; j++, kc++)
                                {
                                    if ((i >= 0 && j >= 0) && (i < num_input_rows && j < num_input_cols)) // in valid region, assuming zero padding
                                    {
                                        int di_i = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, k, i, j);
                                        int f_i = index4D(QUAD_DPTH_SIMD, nKR, nKC, m, k, kr, kc);
                                        outMap[do_i] += (inMaps[di_i] * filters[f_i]);
                                    }
                                }
                            }
                        }
                        outMap[do_i] = (outMap[do_i] < 0.0 && doAct) ? outMap[do_i] * 0.1f : outMap[do_i];
                    }
                }
            }
        },
        t * num_kernels / nthreads,
        (t + 1) == nthreads ? num_kernels : (t + 1) * num_kernels / nthreads,
        t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });
}


void Layer_Job::do_accum(int num_accum_rows, int num_accum_cols, int accum_depth, float* inMapsA, float* inMapsB, float* outMap)
{
    int nthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nthreads);
    for(int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
        {
            for (int m = bi_m; m < ei_m; m++) 
            {
                for (int x = 0; x < num_accum_rows; x++)
                {
                    for (int y = 0; y < num_accum_cols; y++)
                    {
                        int do_i = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, m, x, y);
                        outMap[do_i] = inMapsA[do_i] + inMapsB[do_i];
                    }
                }
            }
        },
        t * accum_depth / nthreads,
        (t + 1) == nthreads ? accum_depth : (t + 1) * accum_depth / nthreads,
        t));
	}
	for_each(threads.begin(), threads.end(), [](std::thread& x){x.join(); });            
}


void Layer_Job::printConfig(Layer_Iteration* lay_it)
{
    FAS_cfg* fas_cfg = lay_it->m_accelCfg->m_FAS_cfg_arr[0];
    cout << "[ESPRESSO]: " << m_layerName                                << endl;
    cout << "[ESPRESSO]:\tFAS_0 - Configuation......."                   << endl;
    cout << "[ESPRESSO]:\tOpcode:                                      " << fas_cfg->m_opcode                   << endl;
    cout << "[ESPRESSO]:\tNum 1x1 Kernels:                             " << fas_cfg->m_num_1x1_kernels          << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Depth:                            " << fas_cfg->m_krnl1x1Depth             << endl;
    cout << "[ESPRESSO]:\tPixel Sequence Configuration Fetch Total:    " << fas_cfg->m_pixSeqCfgFetchTotal      << endl;
    cout << "[ESPRESSO]:\tInput Map Fetch Total:                       " << fas_cfg->m_inMapFetchTotal          << endl;
    cout << "[ESPRESSO]:\tInput Map Fetch Factor:                      " << fas_cfg->m_inMapFetchFactor         << endl;
    cout << "[ESPRESSO]:\tKernel 3x3 Fetch Total:                      " << fas_cfg->m_krnl3x3FetchTotal        << endl;
    cout << "[ESPRESSO]:\tKernel 3x3 Bias Fetch Total:                 " << fas_cfg->m_krnl3x3BiasFetchTotal    << endl;
    cout << "[ESPRESSO]:\tPartial Map Fetch Total:                     " << fas_cfg->m_partMapFetchTotal        << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Fetch Total:                      " << fas_cfg->m_krnl1x1FetchTotal        << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Bias Fetch Total:                 " << fas_cfg->m_krnl1x1BiasFetchTotal    << endl;
    cout << "[ESPRESSO]:\tResidual Map Fetch Total:                    " << fas_cfg->m_resMapFetchTotal         << endl;
    cout << "[ESPRESSO]:\tOutput Map Store Total:                      " << fas_cfg->m_outMapStoreTotal         << endl;
	cout << "[ESPRESSO]:\tNum Output Rows:                             " << fas_cfg->m_num_output_rows			<< endl;
	cout << "[ESPRESSO]:\tNum Output Cols:                             " << fas_cfg->m_num_output_cols			<< endl;
	cout << "[ESPRESSO]:\tOutput Depth:                                " << fas_cfg->m_output_depth				<< endl;
    cout << "[ESPRESSO]:\tPrev1x1 Map Fetch Total                      " << fas_cfg->m_prevMapFetchTotal        << endl;
    cout << "[ESPRESSO]:\tOutput Map Store Factor:                     " << fas_cfg->m_outMapStoreFactor        << endl;
    cout << "[ESPRESSO]:\tConvOut High Watermark:                      " << fas_cfg->m_co_high_watermark        << endl;
    cout << "[ESPRESSO]:\tResdMap Low Watermark:                       " << fas_cfg->m_rm_low_watermark         << endl;
    cout << "[ESPRESSO]:\tPartMap Low Watermark:                       " << fas_cfg->m_pm_low_watermark         << endl;
    cout << "[ESPRESSO]:\tPrev1x1 Low Watermark:                       " << fas_cfg->m_pv_low_watermark         << endl;
    cout << "[ESPRESSO]:\tResdMap Fetch Amount:                        " << fas_cfg->m_rm_fetch_amount          << endl;   
    cout << "[ESPRESSO]:\tPartMap Fetch Amount:                        " << fas_cfg->m_pm_fetch_amount          << endl;
    cout << "[ESPRESSO]:\tPrev1x1 Fetch Amount:                        " << fas_cfg->m_pv_fetch_amount          << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 padding:                          " << fas_cfg->m_krnl1x1_pding            << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Padding begin:                    " << fas_cfg->m_krnl1x1_pad_bgn          << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Padding end:                      " << fas_cfg->m_krnl1x1_pad_end          << endl;
    cout << "[ESPRESSO]:\tActivation:                                  " << fas_cfg->m_act1x1                   << endl;

    for(int a = 0; a < MAX_AWP_PER_FAS; a++)
    {
        AWP_cfg* awp_cfg =  lay_it->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a];
        cout << "[ESPRESSO]:\t\tAWP_id:  " << awp_cfg->m_AWP_id << endl;
        for(int q = 0; q < MAX_QUAD_PER_AWP; q++)
        {
            auto& QUAD_en_arr = lay_it->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
            auto& QUAD_cfg_arr = lay_it->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
            cout << "[ESPRESSO]:\t\t\tQUAD_id: "  << QUAD_cfg_arr[q]->m_QUAD_id << endl;
            if(QUAD_en_arr[q])
            {
                cout << "[ESPRESSO]:\t\t\t\tResult High WaterMark:              " << QUAD_cfg_arr[q]->m_res_high_watermark      << endl;
                cout << "[ESPRESSO]:\t\t\t\tStride:                             " << QUAD_cfg_arr[q]->m_stride                  << endl;
                cout << "[ESPRESSO]:\t\t\t\tNumber of Expanded Input Rows:      " << QUAD_cfg_arr[q]->m_num_expd_input_rows     << endl;
                cout << "[ESPRESSO]:\t\t\t\tNumber of Expanded Input Coloumns:  " << QUAD_cfg_arr[q]->m_num_expd_input_cols     << endl;
                cout << "[ESPRESSO]:\t\t\t\tCropped Input Row Start:            " << QUAD_cfg_arr[q]->m_crpd_input_row_start    << endl;
                cout << "[ESPRESSO]:\t\t\t\tCropped Input Row End:              " << QUAD_cfg_arr[q]->m_crpd_input_row_end      << endl;
                cout << "[ESPRESSO]:\t\t\t\tNumber of Output Rows               " << QUAD_cfg_arr[q]->m_num_output_rows         << endl;
                cout << "[ESPRESSO]:\t\t\t\tNumber of Output Columns::          " << QUAD_cfg_arr[q]->m_num_output_cols         << endl;
                cout << "[ESPRESSO]:\t\t\t\tNumber of Kernels:                  " << QUAD_cfg_arr[q]->m_num_kernels             << endl;
                cout << "[ESPRESSO]:\t\t\t\tMaster QUAD:                        " << QUAD_cfg_arr[q]->m_master_QUAD             << endl;
                cout << "[ESPRESSO]:\t\t\t\tCascade:                            " << QUAD_cfg_arr[q]->m_cascade                 << endl;
                cout << "[ESPRESSO]:\t\t\t\tActivation:                         " << QUAD_cfg_arr[q]->m_act3x3                  << endl;
                cout << "[ESPRESSO]:\t\t\t\tPadding:                            " << QUAD_cfg_arr[q]->m_padding                 << endl;
                cout << "[ESPRESSO]:\t\t\t\tUpsample:                           " << QUAD_cfg_arr[q]->m_upsample                << endl;
            }
            else
            {
                cout << "[ESPRESSO]:\t\t\t\tUnused" << endl;
            }
        }
    }
	
	cout << endl << endl;
}


void Layer_Job::calcAccelPerfAnalyStats(Layer_Iteration* lay_it, double& QUAD_time, double& FAS_time)
{
    // only works properly for unmerged layers
    if(lay_it->m_kernels3x3)
    {
        QUAD_time = ((3 * lay_it->m_inputMaps-> m_numInputMapCols)
                    + ((lay_it->m_inputMaps->m_numInputMapRows - 3) * lay_it->m_inputMaps->m_numInputMapCols)
                    + ((lay_it->m_kernels3x3->m_numKernels / K_3_S) * (lay_it->m_outputMaps->m_numOutputMapRows / R_S) * lay_it->m_outputMaps->m_numOutputMapCols)) * CLK_PRD_NS;
    }
    else if(lay_it->m_kernels1x1)
    {
        FAS_time  = ((lay_it->m_outputMaps->m_numOutputMapRows * lay_it->m_outputMaps->m_numOutputMapCols) 
                    * (lay_it->m_kernels1x1->m_kernelDepth / K_1_D_S) 
                    * (lay_it->m_kernels1x1->m_numKernels / K_1_S)) * CLK_PRD_NS; 
    }
    else
    {
        FAS_time = (lay_it->m_outputMaps->m_numOutputMapRows * lay_it->m_outputMaps->m_numOutputMapCols * (lay_it->m_outputMaps->m_outputMapDepth / A_S)) * CLK_PRD_NS;
    }
}

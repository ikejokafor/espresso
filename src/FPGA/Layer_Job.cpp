#include "Layer_Job.hpp"
using namespace std;
using namespace espresso;


bool isPowerOfTwo(ulong x)
{
    return ((x & (x - 1)) == 0);
}


Layer_Job::Layer_Job(
    string layerName,
    string layerType,
    int group,
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
    activation_t fpgaAct3x3,
    activation_t fpgaAct1x1,
    bool do_kernels1x1,
    FPGA_hndl* fpga_hndl,
    bool krnl_1x1_layer,
    bool do_1x1_res,
    bool do_res_1x1,
    bool first,
    bool last,
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
    m_layerType             = layerType;
    m_group                 = group;
    // delete m_inputMaps;
    // FILE* buf_fd = fopen("/export/home/izo5011/workSpace/espressoTester/12_Residual.bin", "r");
    // float* buf = new float[inputMapDepth * numInputMapRows * numInputMapCols];
    // fread(buf, sizeof(float), inputMapDepth * numInputMapRows * numInputMapCols, buf_fd);
    // m_inputMaps = new InputMaps(fpga_hndl, inputMapDepth, numInputMapRows, numInputMapCols, buf);
    
    
    m_kernels3x3            = new Kernels(fpga_hndl, m_num3x3Kernels, kernelDepth, numKernelRows, numKernelCols, kernel3x3Data);
    if(do_resLayer || m_do_resLayer_only)
    {
        m_residualMaps      = new ResidualMaps(fpga_hndl, residualMapDepth, numResidualMapRows, numResidualMapCols, residualMapData);
    }
    m_kernels3x3Bias        = new KernelBias(fpga_hndl, num3x3Kernels, 3, kernel3x3Bias);
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
        m_kernels1x1Bias    = new KernelBias(fpga_hndl, num1x1Kernels, 1, kernel1x1Bias);
    }
    m_krnl_1x1_layer = krnl_1x1_layer;
	m_fpga_hndl		 = fpga_hndl;   
    m_first          = first;
    m_last           = last;
#ifdef SYSTEMC
    m_sysc_fpga_hndl    = reinterpret_cast<SYSC_FPGA_hndl*>(fpga_hndl);
    m_Spyld = new StatPayload();
	m_Spyld->m_size = ACCL_META_OUTPUT_SIZE;
    m_Spyld->m_buffer = (void*)malloc(m_Spyld->m_size);
    m_sysC_FPGAcfg = new SysC_FPGAconfig();
    m_sysC_FPGAcfg->m_size = 4;   // TODO: hardcoding
    m_sysC_FPGAcfg->m_buffer = (void*)malloc(m_sysC_FPGAcfg->m_size);
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
    m_inputMaps = NULL;
	m_kernels3x3 = NULL;
	m_residualMaps = NULL;
	m_outputMaps = NULL;
	m_kernels3x3Bias = NULL;
    m_kernels1x1 = NULL;
    m_kernels1x1Bias = NULL;


    int i_end = m_lay_it_arr.size();
    for(int i = 0; i < i_end; i++)
    {
        int j_end = m_lay_it_arr[i].size();
        for(int j = 0; j < j_end; j++)
        {
            delete m_lay_it_arr[i][j];
        }
    }

#ifdef SYSTEMC
    (m_Spyld) ? delete m_Spyld : void();
	m_Spyld = NULL;
#endif
}


void Layer_Job::getFabric(int& maxKernels, int& maxDepth, string fabric)
{
    if(fabric == "FPGA")
    {
        maxKernels = ACCL_MAX_KRNLS;
        maxDepth = ACCL_MAX_DEPTH_SIMD;
    }
    else if(fabric == "SSD")
    {
        // maxKernels = ACCL_MAX_KRNLS;
        // maxDepth = ACCL_MAX_DEPTH_SIMD;
    }
}



void Layer_Job::createLayerIters()
{
    m_num_krnl_iter = (m_krnl_1x1_layer || m_do_resLayer_only) ? 1 : ceil((double)m_num3x3Kernels / (double)ACCL_MAX_KRNLS);
    m_num_depth_iter = (m_krnl_1x1_layer|| m_do_resLayer_only) ? 1 : ceil((double)m_inputMapDepth / (double)ACCL_MAX_DEPTH_SIMD);

    // cout << "[ESPRESSO]: Creating Layer Iterations... " << endl;
    // cout << "[ESPRESSO]: " << m_num_krnl_iter << " Kernel Iteration(s)" << endl;
    // cout << "[ESPRESSO]: " << m_num_depth_iter << " Depth Iterations(s)" << endl;
	// cout << endl << endl;

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
            depth = min(ACCL_MAX_DEPTH_SIMD, remDepth);
            layAclPrm = createAccelParams(
                i,
                j,
                depthBgn,
                depth,
                krnlBgn,
                numKrnl
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
                layAclPrm->it_act3x3,
                layAclPrm->it_act1x1,
                layAclPrm->it_bias3x3,
                layAclPrm->it_bias1x1,
                m_krnl1x1_pding,
                m_krnl1x1_pad_bgn,
                m_krnl1x1_pad_end,
                m_layerName,
                i,
                j,
                m_first,
                m_last           
            ));
            remDepth -= depth;
        }
        remNumKrnl -= numKrnl;
    }
    // cout << endl << endl;
}


void Layer_Job::writeLayIt(string outFN, string mode)
{
    FILE* fd = NULL;
    fd = fopen(outFN.c_str(), mode.c_str());
    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            InputMaps* inMaps = m_lay_it_arr[k][d]->m_inputMaps;
            PartialMaps* partMaps = m_lay_it_arr[k][d]->m_partialMaps;
            Kernels* kernels = (m_lay_it_arr[k][d]->m_kernels3x3) ? m_lay_it_arr[k][d]->m_kernels3x3 : m_lay_it_arr[k][d]->m_kernels1x1;
            activation_t act = (m_lay_it_arr[k][d]->m_kernels3x3) ? m_act3x3 : m_act1x1;
            ResidualMaps* resdMaps = m_lay_it_arr[k][d]->m_residualMaps;
            OutputMaps* outMaps = m_lay_it_arr[k][d]->m_outputMaps;
            
            if(m_layerType == "CONVOLUTION" && m_lay_it_arr[k][d]->m_kernels3x3 != NULL)
            {
                fprintf(fd , "%s,%s,%d,%dx%d,%d,%dx%d,%dx%d,%d,%d,%d,%s\n",
                    (m_layerName + "_" + to_string(k) + "_" + to_string(d)).c_str(),
                    m_layerType.c_str(),
                    inMaps->m_depth, inMaps->m_rows, inMaps->m_cols,
                    outMaps->m_depth, outMaps->m_rows, outMaps->m_cols,
                    kernels->m_rows, kernels->m_cols,
                    m_padding, m_stride, m_group, espresso::to_string(act).c_str()
                );
            }
            else if(m_layerType == "CONVOLUTION" && m_lay_it_arr[k][d]->m_kernels1x1 != NULL)
            {
                fprintf(fd , "%s,%s,%d,%dx%d,%d,%dx%d,%dx%d,%d,%d,%d,%s\n",
                    (m_layerName + "_" + to_string(k) + "_" + to_string(d)).c_str(),
                    m_layerType.c_str(),
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    outMaps->m_depth, outMaps->m_rows, outMaps->m_cols,
                    kernels->m_rows, kernels->m_cols,
                    m_padding, m_stride, m_group, espresso::to_string(act).c_str()
                );                
            }
            else if(m_layerType == "RESIDUAL")
            {
                fprintf(fd , "%s,SHORTCUT,%d,%dx%d,%d,%dx%d,-,-,-,-,-\n",
                    m_layerName.c_str(), 
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    outMaps->m_depth, outMaps->m_rows, outMaps->m_cols
                );
            }
        }
    }
    fclose(fd);
}


layAclPrm_t* Layer_Job::createAccelParams(
    int krnl_iter,
    int dpth_iter,
    int depthBgn,
    int depth,
    int krnl3x3Bgn,
    int numKrnl3x3)
{
    layAclPrm_t* layAclPrm = new layAclPrm_t;
    memset(layAclPrm, 0x0, sizeof(layAclPrm_t));
    layAclPrm->inputMaps = (m_krnl_1x1_layer || m_do_resLayer_only) ? NULL : m_inputMaps->GetVolume(depthBgn, depth);
    layAclPrm->kernels3x3 = (m_krnl_1x1_layer || m_do_resLayer_only) ? NULL : m_kernels3x3->GetVolume(krnl3x3Bgn, numKrnl3x3, depthBgn, depth);
    layAclPrm->opcode = (opcode_t)-1;
    bool first_depth_iter = (dpth_iter == 0);
    bool last_depth_iter = (dpth_iter == (m_num_depth_iter - 1));
    bool first_krnl_iter = (krnl_iter == 0);
    bool last_krnl_iter = (krnl_iter == (m_num_krnl_iter - 1));
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
    if(last_depth_iter)
    {
        layAclPrm->it_bias3x3 = true;
    }
    else
    {
        layAclPrm->it_bias3x3 = false;
    }
    //////
    if((last_depth_iter && last_krnl_iter) || m_krnl_1x1_layer)
    {
        layAclPrm->it_bias1x1 = true;
    }
    else
    {
        layAclPrm->it_bias1x1 = false;
    }
    //////
    if(m_act3x3 && last_depth_iter)
    {
        layAclPrm->it_act3x3 = true;
    }
    else
    {
        layAclPrm->it_act3x3 = false;
    }
    //////
    if(m_act1x1 && ((last_depth_iter && last_krnl_iter) || m_krnl_1x1_layer))
    {
        layAclPrm->it_act1x1 = true;
    }
    else
    {
        layAclPrm->it_act1x1 = false;
    }
    //////
    if(last_depth_iter && m_do_resLayer && !m_do_1x1_res)
    {
        layAclPrm->residualMaps = m_residualMaps->GetVolume(krnl3x3Bgn, numKrnl3x3);
    }
    else if((last_depth_iter && m_do_resLayer && m_do_1x1_res && first_krnl_iter) || m_do_resLayer_only)
    {
        layAclPrm->residualMaps =  new ResidualMaps(m_residualMaps);
    }
    //////
    layAclPrm->kernels3x3Bias = (m_krnl_1x1_layer || m_do_resLayer_only || !layAclPrm->it_bias3x3) ? NULL : m_kernels3x3Bias->GetVolume(krnl3x3Bgn, numKrnl3x3);
    //////
    if(m_do_kernels1x1 && last_depth_iter)
    {
        layAclPrm->kernels1x1 = (m_krnl_1x1_layer) ? new Kernels(m_kernels1x1) : m_kernels1x1->GetVolume(0, m_kernels1x1->m_numKernels, krnl3x3Bgn, numKrnl3x3);
        layAclPrm->kernels1x1Bias = (!layAclPrm->it_bias1x1) ? NULL : m_kernels1x1Bias;
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
    if(m_do_resLayer_only)
    {
        layAclPrm->outputMaps = new OutputMaps(m_fpga_hndl, m_residualMaps->m_depth, m_residualMaps->m_rows, m_residualMaps->m_cols);
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
        layAclPrm->partialMaps = new PartialMaps(m_fpga_hndl, m_inputMaps->m_depth / 2, m_inputMaps->m_rows, m_inputMaps->m_cols, m_inputMaps->m_cpu_data);
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


void Layer_Job::process(double& elapsed_time, double& QUAD_time, double& FAS_time)
{
    elapsed_time = 0.0f;
    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            calcAccelPerfAnalyStats(m_lay_it_arr[k][d], QUAD_time, FAS_time);
            elapsed_time += max(QUAD_time, FAS_time);
        }
    }
}


#ifdef SYSTEMC
typedef std::numeric_limits< double > dbl;
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
            m_lay_it_arr[k][d]->prepFPGAData();
            if(k == 0 && d == 0)
            {
                calcAccelPerfAnalyStats(m_lay_it_arr[k][d], avg_QUAD_time0, avg_FAS_time0);
            }
			cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tProcessing Kernel Iteration - " << (k + 1) << "/" << m_num_krnl_iter << endl;
            cout << "[ESPRESSO]:\tProcessing Depth Iteration - " << (d + 1)  << "/" << m_num_depth_iter << endl;
			cout << endl << endl;
            printConfig(m_lay_it_arr[k][d]);
            
            // SystemC Config
            (*(int*)m_sysC_FPGAcfg->m_buffer) = (int)1;
            // (*(int*)m_sysC_FPGAcfg->m_buffer) = m_max_sys_trans;
            m_sysc_fpga_hndl->wr_sysC_FPGAconfig(m_sysC_FPGAcfg);
            m_sysc_fpga_hndl->wrConfig(m_lay_it_arr[k][d]->m_accelCfg);
            // (m_lay_it_arr[k][d]->m_inputMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_inputMaps)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_kernels3x3) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels3x3)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_kernels3x3Bias) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels3x3Bias)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_kernels1x1) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels1x1)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_kernels1x1Bias) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_kernels1x1Bias)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_partialMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_partialMaps)
			// 	: m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_residualMaps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_residualMaps)
            //     : m_sysc_fpga_hndl->wrParam(m_Dpyld);
            // (m_lay_it_arr[k][d]->m_prev1x1Maps) ? m_sysc_fpga_hndl->wrParam(m_lay_it_arr[k][d]->m_prev1x1Maps)
            //     : m_sysc_fpga_hndl->wrParam(m_Dpyld);
            m_sysc_fpga_hndl->sendStart();
            m_sysc_fpga_hndl->waitComplete();
            // m_sysc_fpga_hndl->getOutput(m_lay_it_arr[k][d]->m_outputMaps);
            cout << endl << endl;
            // get output stats           
			m_sysc_fpga_hndl->getOutput(m_Spyld);
            cout << endl << endl;
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
    cout.precision(dbl::max_digits10);
	cout << "[ESPRESSO]: Total Layer Processing Time - " << elapsed_time << " ns " << endl;
    cout.precision(dbl::max_digits10);
    cout << "[ESPRESSO]: Avgerage Layer Iteration Time - " << avgIterTime << " ns " << endl;
    cout << "[ESPRESSO]: Total Power Consumed - " << memPower << " mW " << endl;
	cout << endl << endl;
	delete m_Dpyld;
    m_Dpyld = NULL;
    delete m_sysC_FPGAcfg;
    m_sysC_FPGAcfg = NULL;
    m_sysc_fpga_hndl->resetMemSpace();
}
#endif











void printMaps(string fn, float* buf, int depth, int rows, int cols, bool sel)
{
    FILE *fd = fopen(fn.c_str(), "w");
    for(int d = 0; d < depth; d++)
    {
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < cols; c++)
            {
                int idx = (sel) ? index3D(rows, cols, d, r, c)
                    : index3D(MAX_INPUT_ROWS, MAX_INPUT_COLS, d, r, c);
                fprintf(fd, "%f ", buf[idx]);
            }
            fprintf(fd, "\n");
        }
        fprintf(fd, "\n\n\n");
    }
    fclose(fd);
}


void printKrnls(string fn, float* buf, int num, int depth, int rows, int cols)
{
    FILE *fd = fopen(fn.c_str(), "w");
	for(int n = 0; n < num; n++)
    {
        for(int d = 0; d < depth; d++)
        {
            for(int r = 0; r < rows; r++)
            {
                for(int c = 0; c < cols; c++)
                {
                    int idx = index4D(SYSC_MAX_KRNL_DEPTH, MAX_KRNL_SIZE, MAX_KRNL_SIZE, n, d, r, c);
                    fprintf(fd, "%f ", buf[idx]);
                }
                fprintf(fd, "\n");
            }
            fprintf(fd, "\n\n\n");
        }
    }
    fclose(fd);
}


void printKrnls(string fn, krnl_data_t& krnl_data, int num, int depth, int rows, int cols)
{
    FILE *fd = fopen(fn.c_str(), "w");
	for(int n = 0; n < num; n++)
    {
        for(int d = 0; d < depth; d++)
        {
            for(int r = 0; r < rows; r++)
            {
                for(int c = 0; c < cols; c++)
                {
                    int idx = index2D(rows, r, c);
                    fprintf(fd, "%f ", krnl_data[n][d][idx]);
                }
                fprintf(fd, "\n");
            }
            fprintf(fd, "\n\n\n");
        }
    }
    fclose(fd);
}


void printKBias(string fn, float* buf, int num)
{
    FILE *fd = fopen(fn.c_str(), "w");
	for(int n = 0; n < num; n++)
    {
        fprintf(fd, "%f\n", buf[n]);
    }
    fclose(fd);
}








int Layer_Job::lasQD(Layer_Iteration* lay_it)
{
    for(int i = NUM_TOTAL_QUADS - 1; i >= 0; i--)
    {
        if(lay_it->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_en_arr[i])
        {
            return i;
        }
    }
    return 0;
}


void Layer_Job::process(float* layOut)
{
    string prefix = "/export/home/izo5011/workSpace/espressoTester/";
    // printMaps(prefix + "inputMaps_fpga.txt" , (float*)m_inputMaps->m_cpu_data, m_inputMaps->m_depth, m_inputMaps->m_rows, m_inputMaps->m_cols, true);
    // printKrnls(prefix + "kernels_fpga.txt", m_kernels3x3->m_cpu_data, m_kernels3x3->m_numKernels, m_kernels3x3->m_depth, 3, 3);
    // printKBias(prefix + "kernel_bias_fpga.txt", (float*)m_kernels3x3Bias->m_cpu_data, m_kernels3x3Bias->m_numKernels);

    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            m_lay_it_arr[k][d]->prepFPGAData();
            cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tProcessing Kernel Iteration - " << (k + 1) << "/" << m_num_krnl_iter << endl;
            cout << "[ESPRESSO]:\tProcessing Depth Iteration - " << (d + 1)  << "/" << m_num_depth_iter << endl;
			cout << endl << endl;

            printConfig(m_lay_it_arr[k][d]);
            int stride = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_stride;
            int upsample = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_upsample;
            int padding = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_padding;
            activation_t act3x3 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_act3x3;
            activation_t act1x1 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_act1x1;
            int lasQD_i = lasQD(m_lay_it_arr[k][d]);
            bool it_act3x3 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[lasQD_i]->m_it_act3x3;
            bool it_act1x1 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_it_act1x1;
            bool it_bias3x3 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[lasQD_i]->m_it_bias3x3;
            bool it_bias1x1 = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_it_bias1x1;
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
            int depth = max(FAS_MAX_1X1_KRNLS, ACCL_MAX_DEPTH_SIMD);
            float* intmStrgA = new float[depth * MAX_INPUT_ROWS * MAX_INPUT_COLS];
            float* intmStrgB = new float[depth * MAX_INPUT_ROWS * MAX_INPUT_COLS];
            int qd_nOutRows = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_num_output_rows;
            int qd_nOutCols = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[0]->m_QUAD_cfg_arr[0]->m_num_output_cols;


            if(upsample)
            {
                UpSample(stride, inMaps->m_cpu_data, inMaps->m_depth, inMaps->m_rows, inMaps->m_cols, intmStrgA);
            }
            else if(opcode == OPCODE_14 || opcode == OPCODE_17)
            {
                esp_copy_2IntS(
                    partMaps->m_cpu_data,
                    partMaps->m_depth,
                    partMaps->m_rows,
                    partMaps->m_cols,
                    intmStrgA
                );
            }
            else
            {
                esp_copy_2IntS(
                    (float*)inMaps->m_cpu_data,
                    inMaps->m_depth,
                    inMaps->m_rows,
                    inMaps->m_cols,
                    intmStrgA
                );
            }


            // printMaps(prefix + "inputMaps_fpga_" + to_string(k) + "_" + to_string(d) + ".txt", (float*)inMaps->m_cpu_data, inMaps->m_depth, inMaps->m_rows, inMaps->m_cols, true);
            // printKrnls(prefix + "kernels_fpga_" + to_string(k) + "_" + to_string(d) + ".txt", kernels3x3->m_cpu_data, kernels3x3->m_numKernels, kernels3x3->m_depth, 3, 3);
            // (it_bias3x3) ? printKBias(prefix + "kernel_bias_fpga_" + to_string(k) + "_" + to_string(d) + ".txt", kernels3x3Bias->m_cpu_data, kernels3x3Bias->m_numKernels) : void();
            // printMaps(prefix + "intmStrgA_" + to_string(k) + "_" + to_string(d) + ".txt" , intmStrgA, inMaps->m_depth, MAX_INPUT_ROWS, MAX_INPUT_COLS, false);


            if(opcode == OPCODE_0)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_1)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_2)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, kernels1x1Bias->m_cpu_data, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_3)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, kernels1x1Bias->m_cpu_data, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_4)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, kernels1x1Bias->m_cpu_data, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_5)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_6)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                 do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_7)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_8)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_9)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_10)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_11)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_12)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_13)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                do_conv( // 1x1 conv
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB,
                    1, 0, false, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA
                );
                do_accum( // prev1x1Map accum
                    act1x1, it_act1x1,
                    prev1x1Maps->m_cpu_data,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    prev1x1Maps->m_depth, prev1x1Maps->m_rows, prev1x1Maps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_14)
            {
                do_conv( // 1x1 conv
                    partMaps->m_rows, partMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    1, 0, it_act1x1, act1x1,
                    1, 1, kernels1x1->m_depth,
                    kernels1x1->m_numKernels, kernels1x1->m_cpu_data, (kernels1x1Bias) ? kernels1x1Bias->m_cpu_data : NULL, it_bias1x1,
                    outMaps->m_rows, outMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_15)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, false, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );
                
                // printMaps(prefix + "partialMaps_" + to_string(k) + "_" + to_string(d) + ".txt", partMaps->m_cpu_data, partMaps->m_depth, partMaps->m_rows, partMaps->m_cols, true);

                do_accum( // partMaps accum
                    act3x3, it_act3x3,
                    partMaps->m_cpu_data,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    partMaps->m_depth, partMaps->m_rows, partMaps->m_cols
                );

                esp_copy_2Buf(
                    intmStrgA,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);

                // printMaps(prefix + "outMaps_" + to_string(k) + "_" + to_string(d) + ".txt", outMaps->m_cpu_data, outMaps->m_depth, outMaps->m_rows, outMaps->m_cols, true);
            }
            else if(opcode == OPCODE_16)
            {
                do_conv( // 3x3 conv
                    inMaps->m_rows, inMaps->m_cols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgA,
                    stride, padding, it_act3x3, act3x3,
                    3, 3, kernels3x3->m_depth,
                    kernels3x3->m_numKernels, kernels3x3->m_cpu_data, (kernels3x3Bias) ? kernels3x3Bias->m_cpu_data : NULL, it_bias3x3,
                    qd_nOutRows, qd_nOutCols, MAX_INPUT_ROWS, MAX_INPUT_COLS, intmStrgB
                );

                // printMaps(prefix + "intmStrgB_" + to_string(k) + "_" + to_string(d) + ".txt" , intmStrgB, kernels3x3->m_numKernels, qd_nOutRows, qd_nOutCols, false);

                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );

                // printMaps(prefix + "outMaps_" + to_string(k) + "_" + to_string(d) + ".txt" , outMaps->m_cpu_data, outMaps->m_depth, outMaps->m_rows, outMaps->m_cols, true);

                free(intmStrgA);
                free(intmStrgB);
            }
            else if(opcode == OPCODE_17)
            {
                do_accum( // resd accum
                    activation_t(), false,
                    resdMaps->m_cpu_data,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols,
                    intmStrgA,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    intmStrgB,
                    depth, MAX_INPUT_ROWS, MAX_INPUT_COLS,
                    resdMaps->m_depth, resdMaps->m_rows, resdMaps->m_cols
                );
                esp_copy_2Buf(
                    intmStrgB,
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols
                );
                free(intmStrgA);
                free(intmStrgB);
            }
            cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tFinished Kernel Iteration - " << (k + 1) << "/" << m_num_krnl_iter << endl;
            cout << "[ESPRESSO]:\tFinished Depth Iteration - "  << (d + 1) << "/" << m_num_depth_iter << endl;
            cout << endl << endl;
            if(d != (m_lay_it_arr[k].size() - 1) && m_lay_it_arr[k].size() > 0)
            {
                esp_copy(
                    outMaps->m_cpu_data,
                    outMaps->m_depth,
                    outMaps->m_rows,
                    outMaps->m_cols,
                    m_lay_it_arr[k][d + 1]->m_partialMaps->m_cpu_data,
                    m_lay_it_arr[k][d + 1]->m_partialMaps->m_depth, m_lay_it_arr[k][d + 1]->m_partialMaps->m_rows, m_lay_it_arr[k][d + 1]->m_partialMaps->m_cols,
                    m_lay_it_arr[k][d + 1]->m_partialMaps->m_depth, m_lay_it_arr[k][d + 1]->m_partialMaps->m_rows, m_lay_it_arr[k][d + 1]->m_partialMaps->m_cols
                );
            }
        }
    }

    int i = 0;
    int j = 0;
    float* ptr = layOut;
    int j_end = m_outputMapDepth * m_numOutputMapRows * m_numOutputMapCols;
    for(i = 0; i < m_num_krnl_iter; i++)
    {
        int d_end = m_lay_it_arr[i].size() - 1;
        int it_outputMapDepth = m_lay_it_arr[i][d_end]->m_outputMaps->m_depth;
        int it_numOutputMapRows = m_lay_it_arr[i][d_end]->m_outputMaps->m_rows;
        int it_numOutputMapCols = m_lay_it_arr[i][d_end]->m_outputMaps->m_cols;
        ptr += j;
        esp_copy(
            (float*)m_lay_it_arr[i][d_end]->m_outputMaps->m_cpu_data,
            it_outputMapDepth,
            it_numOutputMapRows,
            it_numOutputMapCols,
            ptr,
            m_outputMapDepth, m_numOutputMapRows, m_numOutputMapCols,
            it_outputMapDepth, it_numOutputMapRows, it_numOutputMapCols
        );
        ptr += (it_outputMapDepth * it_numOutputMapRows * it_numOutputMapCols);
    }
    printMaps(prefix + m_layerName + ".txt", layOut, m_outputMapDepth, m_numOutputMapRows, m_numOutputMapCols, true);
    FILE* buf_fd = fopen((prefix + m_layerName + ".bin").c_str(), "w");
    fwrite((const char*)layOut, sizeof(float), m_outputMapDepth * m_numOutputMapRows * m_numOutputMapCols, buf_fd);
    fclose(buf_fd);
    // Debug ----------------------------------------------------------
}


void Layer_Job::esp_copy(float* src, int sDepth, int nSRows, int nSCols, float* dst, int dDepth, int nDRows, int nDCols, int depth, int nRows, int nCols)
{
    for(int k = 0; k < depth; k++)
    {
        for(int i = 0; i < nRows; i++)
        {
            for(int j = 0; j < nCols; j++)
            {
                int sidx = index3D(nSRows, nSCols, k, i, j);
                int didx = index3D(nDRows, nDCols, k, i, j);
                dst[didx] = src[sidx];
            }
        }
    }
}


void Layer_Job::esp_copy_2IntS(float* src, int sDepth, int nSRows, int nSCols, float* intS)
{
    for(int k = 0; k < sDepth; k++)
    {
        for(int i = 0; i < nSRows; i++)
        {
            for(int j = 0; j < nSCols; j++)
            {
                int sidx = index3D(nSRows, nSCols, k, i, j);
                int didx = index3D(MAX_INPUT_ROWS, MAX_INPUT_COLS, k, i, j);
                intS[didx] = src[sidx];
            }
        }
    }
}


void Layer_Job::esp_copy_2Buf(float* intS, float* buf, int dDepth, int nDRows, int nDCols)
{
    for(int k = 0; k < dDepth; k++)
    {
        for(int i = 0; i < nDRows; i++)
        {
            for(int j = 0; j < nDCols; j++)
            {
                int sidx = index3D(MAX_INPUT_ROWS, MAX_INPUT_COLS, k, i, j);
                int didx = index3D(nDRows, nDCols, k, i, j);
                buf[didx] = intS[sidx];
            }
        }
    }
}


void Layer_Job::UpSample(
    int stride, 
    float* inMaps, 
    int inputDepth, 
    int numInputRows, 
    int numInputCols, 
    float* outMap)
{
	for(int k = 0; k < inputDepth; ++k)
    {
		for(int j = 0; j < numInputRows * stride; ++j)
        {
			for(int i = 0; i < numInputCols * stride; ++i)
            {
				int in_index = k * numInputRows * numInputRows + (j / stride) * numInputRows + i / stride;
				int out_index = k * numInputRows * numInputRows * stride * stride + j * numInputRows * stride + i;
				outMap[out_index] = inMaps[in_index];
			}
		}
	}
}


void Layer_Job::do_conv(
    int num_input_rows, int num_input_cols, int num_InBuf_rows, int num_InBuf_cols, float* inMaps, 
    int stride, int padding, bool it_act, espresso::activation_t act,
    int nKR, int nKC, int kernelDepth, 
    int num_kernels, krnl_data_t filters, float* bias, bool doBias,
    int num_output_rows, int num_output_cols, int num_outBuf_rows, int num_outBuf_cols, float* outMap)
{
    int nthreads = std::thread::hardware_concurrency();
    // int nthreads = 1;
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
                        int do_i = index3D(num_outBuf_rows, num_outBuf_cols, m, x, y);
                        outMap[do_i] = (doBias) ? bias[m] : 0.0f;
                        for (int k = 0; k < kernelDepth; k++)
                        {
                            for (int i = a - padding, kr = 0; kr < nKR; i++, kr++)
                            {
                                for (int j = b - padding, kc = 0; kc < nKC; j++, kc++)
                                {
                                    if ((i >= 0 && j >= 0) && (i < num_input_rows && j < num_input_cols)) // in valid region, assuming zero padding
                                    {
                                        int di_i = index3D(num_InBuf_rows, num_InBuf_cols, k, i, j);
                                        int f_i = index2D(MAX_KRNL_SIZE, kr, kc);
                                        outMap[do_i] += (inMaps[di_i] * filters[m][k][f_i]);
                                    }
                                }
                            }
                        }
                        outMap[do_i] = (outMap[do_i] < 0.0f && it_act && act == espresso::LEAKY) ? outMap[do_i] * 0.1f
                                        :   (outMap[do_i] < 0.0f && it_act && act == espresso::RELU) ? 0.0f
                                        :   outMap[do_i];
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


void Layer_Job::do_accum(
    espresso::activation_t act, bool it_act,
    float* inMapsA, 
    int a_depth, int a_rows, int a_cols,
    float* inMapsB,
    int b_depth, int b_rows, int b_cols,
    float* outMaps,
    int o_depth, int o_rows, int o_cols,
    int depth, int rows, int cols)
{
    int nthreads = std::thread::hardware_concurrency();
    // int nthreads = 1;
    std::vector<std::thread> threads(nthreads);
    for(int t = 0; t < nthreads; t++)
	{
		threads[t] = std::thread(std::bind(
		[&](const int bi_m, const int ei_m, const int t)
        {
            for (int m = bi_m; m < ei_m; m++)
            {
                for (int x = 0; x < rows; x++)
                {
                    for (int y = 0; y < cols; y++)
                    {
                        int a_i = index3D(a_rows, a_cols, m, x, y);
                        int b_i = index3D(b_rows, b_cols, m, x, y);
                        int o_i = index3D(o_rows, o_cols, m, x, y);
                        outMaps[o_i] = inMapsA[a_i] + inMapsB[b_i];
                        outMaps[o_i] = (outMaps[o_i] < 0.0f && it_act && act == espresso::LEAKY) ? outMaps[o_i] * 0.1f
                                        :   (outMaps[o_i] < 0.0f && it_act && act == espresso::RELU) ? 0.0f
                                        :   outMaps[o_i];
                    }
                }
            }
        },
        t * depth / nthreads,
        (t + 1) == nthreads ? depth : (t + 1) * depth / nthreads,
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
    cout << "[ESPRESSO]:\tInput Map Fetch Address:                     " << fas_cfg->m_inMapAddr                << endl;
    cout << "[ESPRESSO]:\tInput Map Fetch Amt:                         " << fas_cfg->m_inMapFetchAmt            << endl;
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
    cout << "[ESPRESSO]:\tOutput Map Store Vld Total                   " << fas_cfg->m_om_store_vld_total       << endl;
    cout << "[ESPRESSO]:\tPrev1x1 Map Fetch Total                      " << fas_cfg->m_prevMapFetchTotal        << endl;
    cout << "[ESPRESSO]:\tOutput Map Store Factor:                     " << fas_cfg->m_outMapStoreFactor        << endl;
    cout << "[ESPRESSO]:\tConvOut High Watermark:                      " << fas_cfg->m_co_high_watermark        << endl;
    cout << "[ESPRESSO]:\tResdMap Low Watermark:                       " << fas_cfg->m_rm_low_watermark         << endl;
    cout << "[ESPRESSO]:\tPartMap Low Watermark:                       " << fas_cfg->m_pm_low_watermark         << endl;
    cout << "[ESPRESSO]:\tPrev1x1 Low Watermark:                       " << fas_cfg->m_pv_low_watermark         << endl;
    cout << "[ESPRESSO]:\tResdMap Fetch Vld Total:                     " << fas_cfg->m_rm_ftch_vld_total        << endl;
    cout << "[ESPRESSO]:\tPartMap Fetch Vld Total:                     " << fas_cfg->m_pm_ftch_vld_total        << endl;
    cout << "[ESPRESSO]:\tPrev1x1 Fetch Vld Total:                     " << fas_cfg->m_pv_ftch_vld_total        << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 padding:                          " << fas_cfg->m_krnl1x1_pding            << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Padding begin:                    " << fas_cfg->m_krnl1x1_pad_bgn          << endl;
    cout << "[ESPRESSO]:\tKernel 1x1 Padding end:                      " << fas_cfg->m_krnl1x1_pad_end          << endl;
    cout << "[ESPRESSO]:\tActivation Type:                             " << fas_cfg->m_act1x1                   << endl;
    cout << "[ESPRESSO]:\tActivate:                                    " << fas_cfg->m_it_act1x1                << endl;
    cout << "[ESPRESSO]:\tDo Bias:                                     " << fas_cfg->m_it_bias1x1               << endl;

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
                cout << "[ESPRESSO]:\t\t\t\tActivation Type:                    " << QUAD_cfg_arr[q]->m_act3x3                  << endl;
                cout << "[ESPRESSO]:\t\t\t\tActivate:                           " << QUAD_cfg_arr[q]->m_it_act3x3               << endl;
                cout << "[ESPRESSO]:\t\t\t\tPadding:                            " << QUAD_cfg_arr[q]->m_padding                 << endl;
                cout << "[ESPRESSO]:\t\t\t\tUpsample:                           " << QUAD_cfg_arr[q]->m_upsample                << endl;
                cout << "[ESPRESSO]:\t\t\t\tDo Bias:                            " << QUAD_cfg_arr[q]->m_it_bias3x3              << endl;
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
        QUAD_time = ((3 * lay_it->m_inputMaps->m_cols)
                    + ((lay_it->m_inputMaps->m_rows - 3) * lay_it->m_inputMaps->m_cols) // bc we dont overlap PFB loading and execution
                    + ((lay_it->m_kernels3x3->m_numKernels / K_3_S) * (lay_it->m_outputMaps->m_rows / R_S) * (lay_it->m_outputMaps->m_cols / MX_3X3_S))) * CLK_PRD_NS;
    }
    else if(lay_it->m_kernels1x1)
    {
        FAS_time  = ((lay_it->m_outputMaps->m_rows * (lay_it->m_outputMaps->m_cols / MX_1X1_S))
                    * (lay_it->m_kernels1x1->m_depth / K_1_D_S)
                    * (lay_it->m_kernels1x1->m_numKernels / K_1_S)) * CLK_PRD_NS;
    }
    else
    {
        FAS_time = (lay_it->m_outputMaps->m_rows * (lay_it->m_outputMaps->m_cols / MX_1X1_S) * (lay_it->m_outputMaps->m_depth / A_S)) * CLK_PRD_NS;
    }
}

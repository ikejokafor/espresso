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
    bool activation,
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
	m_pyld					= NULL;
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
    m_activation            = activation        ;
    m_do_kernels1x1         = do_kernels1x1     ;
    m_krnl1x1_pding         = false             ;
    m_do_1x1_res            = do_1x1_res        ;
    m_do_res_1x1            = do_res_1x1        ;
    m_inputMaps             = new InputMaps(inputMapDepth, numInputMapRows, numInputMapCols, inputMapData);
    m_kernels3x3            = new Kernels(m_num3x3Kernels, kernelDepth, numKernelRows, numKernelCols, kernel3x3Data);
    if(do_resLayer || m_do_resLayer_only)
    {
        m_residualMaps      = new ResidualMaps(residualMapDepth, numResidualMapRows, numResidualMapCols, residualMapData);
    }
    m_kernels3x3Bias        = new KernelBias(num3x3Kernels, kernel3x3Bias);
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
        m_kernels1x1        = new Kernels(num1x1Kernels, m_kernel1x1Depth, 1, 1, kernel1x1Data);
        m_kernels1x1Bias    = new KernelBias(num1x1Kernels, kernel1x1Bias);
    }
    m_krnl_1x1_layer = krnl_1x1_layer;
    m_pyld = new DummyPayload();
    m_pyld->m_address = (uint64_t)malloc(sizeof(ACCL_OUTPUT_SIZE));
    m_pyld->m_size = ACCL_OUTPUT_SIZE;
#ifdef SYSTEMC
    int m_pseudo_addr   = 0;
    m_sysc_fpga_hndl    = reinterpret_cast<SYSC_FPGA_hndl*>(fpga_hndl);
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
	(m_pyld) ? delete m_pyld : void();
    m_inputMaps = NULL;
	m_kernels3x3 = NULL;
	m_residualMaps = NULL;
	m_outputMaps = NULL;
	m_kernels3x3Bias = NULL;
    m_kernels1x1 = NULL;
    m_kernels1x1Bias = NULL;
	m_pyld = NULL;

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
    m_num_krnl_iter = (m_krnl_1x1_layer || m_do_resLayer_only) ? 1 : ceil((double)m_num3x3Kernels / (double)QUAD_MAX_KERNELS);
    m_num_depth_iter = (m_krnl_1x1_layer|| m_do_resLayer_only) ? 1 : ceil((double)m_inputMapDepth / (double)QUAD_DPTH_SIMD);
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
                m_activation,
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
            layAclPrm->outputMaps = new OutputMaps(m_kernels1x1->m_numKernels, m_numOutputMapRows, m_numOutputMapCols);
        }
        else
        {
            layAclPrm->outputMaps = new OutputMaps(m_kernels1x1->m_numKernels + (m_krnl1x1_pad_end - m_krnl1x1_pad_bgn), m_numOutputMapRows, m_numOutputMapCols);
        }
    }
    else
    {
        layAclPrm->outputMaps = new OutputMaps(numKrnl3x3, m_numOutputMapRows, m_numOutputMapCols);
    }
    //////
    if(m_do_resLayer_only)
    {
        layAclPrm->outputMaps = new OutputMaps(m_residualMaps->m_residualMapDepth, m_residualMaps->m_numResidualMapRows, m_residualMaps->m_numResidualMapCols);
    }
    //////
    if((m_do_1x1_res || m_do_res_1x1 || m_do_kernels1x1) && !m_krnl_1x1_layer && last_depth_iter && !first_krnl_iter)
    {
        layAclPrm->prev1x1maps = new Prev1x1Maps(m_lay_it_arr[krnl_iter - 1][m_num_depth_iter - 1]->m_outputMaps);
    }
    //////
    if(m_krnl_1x1_layer)
    {
        layAclPrm->partialMaps = new PartialMaps(m_inputMaps);
    }
    else if(m_do_resLayer_only)
    {
        layAclPrm->partialMaps = new PartialMaps(m_inputMaps->m_inputMapDepth / 2, m_inputMaps->m_numInputMapRows, m_inputMaps->m_numInputMapCols, m_inputMaps->m_data);
    }
    else if(first_depth_iter)
    {
        layAclPrm->partialMaps = NULL;
    }
    else
    {
        layAclPrm->partialMaps = new PartialMaps(m_lay_it_arr[krnl_iter][dpth_iter - 1]->m_outputMaps);
    }
    return layAclPrm;
}


void Layer_Job::process(double& elapsed_time, double& avgIterTime, double& memPower, double& avg_QUAD_time0, double& avg_FAS_time0, double& avg_QUAD_time1, double& avg_FAS_time1)
{
    cout << "[ESPRESSO]: " << m_num_krnl_iter << " Kernel Iteration(s)" << endl;
    cout << "[ESPRESSO]: " << m_num_depth_iter << " Depth Iterations(s)" << endl;
    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for(int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            printConfig(m_lay_it_arr[k][d]);
            calcAccelPerfAnalyStats(m_lay_it_arr[k][d], avg_QUAD_time0, avg_FAS_time0);
            cout << "[ESPRESSO]: " << m_layerName                      << endl;
            cout << "[ESPRESSO]:\tProcessing Kernel Iteration - " << (k + 1) << "/" << m_num_krnl_iter << endl;
            cout << "[ESPRESSO]:\tProcessing Depth Iteration - " << (d + 1)  << "/" << m_num_depth_iter << endl;
            m_sysc_fpga_hndl->setConfig(m_lay_it_arr[k][d]->m_accelCfg);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_inputMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_kernels3x3);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_kernels1x1);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_partialMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_residualMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_outputMaps);
            m_sysc_fpga_hndl->sendStart();
            m_sysc_fpga_hndl->waitComplete();
            m_sysc_fpga_hndl->getOutput(reinterpret_cast<Accel_Payload*>(m_pyld));
            double* ptr = (double*)m_pyld->m_address;
            elapsed_time += (ptr[0]);
            memPower += (ptr[1]);
            avg_QUAD_time1 += (ptr[2]);
            avg_FAS_time1 += (ptr[3]);
            cout << "[ESPRESSO]: " << m_layerName                    << endl;
            cout << "[ESPRESSO]:\tFinished Kernel Iteration - " << (k + 1) << endl;
            cout << "[ESPRESSO]:\tFinished Depth Iteration - "  << (d + 1) << endl;
        }
    }
	double numTotalIter = m_num_krnl_iter * m_num_depth_iter;
	avgIterTime = elapsed_time / numTotalIter;
    avg_QUAD_time1 /= numTotalIter;
    avg_FAS_time1 /= numTotalIter;
	cout << "[ESPRESSO]: Total Layer Processing Time - " << elapsed_time << " ns " << endl;
    cout << "[ESPRESSO]: Avgerage Layer Iteration Time - " << avgIterTime << " ns " << endl;
    cout << "[ESPRESSO]: Total Power Consumed - " << memPower << " mW " << endl;
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
                cout << "[ESPRESSO]:\t\t\t\tActivation:                         " << QUAD_cfg_arr[q]->m_activation              << endl;
                cout << "[ESPRESSO]:\t\t\t\tPadding:                            " << QUAD_cfg_arr[q]->m_padding                 << endl;
                cout << "[ESPRESSO]:\t\t\t\tUpsample:                           " << QUAD_cfg_arr[q]->m_upsample                << endl;
            }
            else
            {
                cout << "[ESPRESSO]:\t\t\t\tUnused" << endl;
            }
        }
    }
}


void Layer_Job::calcAccelPerfAnalyStats(Layer_Iteration* lay_it, double& QUAD_time, double& FAS_time)
{
    // only works properly for unmerged layers
    if(lay_it->m_kernels3x3)
    {
        QUAD_time = (3 * lay_it->m_inputMaps-> m_numInputMapCols)
                    + ((lay_it->m_inputMaps->m_numInputMapRows - 3) * lay_it->m_inputMaps->m_numInputMapCols)
                    + (lay_it->m_kernels3x3->m_numKernels * lay_it->m_outputMaps->m_numOutputMapRows * lay_it->m_outputMaps->m_numOutputMapCols);
    }
    else if(lay_it->m_kernels1x1)
    {
        int K_1_D_S = 32;
        int K_1_S = 1;
        FAS_time  = (lay_it->m_outputMaps->m_numOutputMapRows * lay_it->m_outputMaps->m_numOutputMapCols) 
                    * (lay_it->m_kernels1x1->m_kernelDepth / K_1_D_S) 
                    * (lay_it->m_kernels1x1->m_numKernels / K_1_S); 
    }
    else
    {
        int A_S = 32;
        FAS_time = (lay_it->m_outputMaps->m_numOutputMapRows * lay_it->m_outputMaps->m_numOutputMapCols * lay_it->m_outputMaps->m_outputMapDepth) / A_S;
    }
}

#include "Layer_Job.hpp"
using namespace std;


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
    bool do_res_layer,
    bool activation,
    bool do_kernels1x1,
    FPGA_hndl* fpga_hndl,
    int fxPtLength,
    int numFracBits
) {
    m_inputMaps             = nullptr;
    m_kernels3x3            = nullptr;
    m_residualMaps          = nullptr;
    m_outputMaps            = nullptr;
    m_kernels3x3Bias        = nullptr;
    m_kernels1x1            = nullptr;
    m_kernels1x1Bias        = nullptr;
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
    m_do_res_layer          = do_res_layer      ;
    m_activation            = activation        ;
    m_do_kernels1x1         = do_kernels1x1     ;
    m_inputMaps             = new InputMaps(inputMapDepth, numInputMapRows, numInputMapCols, inputMapData);
    m_kernels3x3            = new Kernels(m_num3x3Kernels, kernelDepth, numKernelRows, numKernelCols, kernel3x3Data);
    if(do_res_layer)
    {
        m_residualMaps      = new ResidualMaps(residualMapDepth, numResidualMapRows, numResidualMapCols, residualMapData);
    }
    m_kernels3x3Bias        = new KernelBias(num3x3Kernels, kernel3x3Bias);
    if(do_kernels1x1)
    {
        m_kernels1x1        = new Kernels(num1x1Kernels, m_kernel1x1Depth, 1, 1, kernel1x1Data);
        m_kernels1x1Bias    = new KernelBias(num1x1Kernels, kernel1x1Bias);
    }
#ifdef SYSTEMC
    m_sysc_fpga_hndl        = reinterpret_cast<SYSC_FPGA_hndl*>(fpga_hndl);
#else

#endif
}


Layer_Job::~Layer_Job()
{
    int i_end = m_lay_it_arr.size();
    for (int i = 0; i < i_end; i++)
    {
        int j_end = m_lay_it_arr[i].size();
        for(int j = 0; j < j_end; j++)
        {
            delete m_lay_it_arr[i][j];
        }
    }
    delete m_inputMaps;
    delete m_kernels3x3;
    if(m_do_res_layer)
    {
        delete m_residualMaps;
    }
    delete m_outputMaps;
    delete m_kernels3x3Bias;
    if(m_do_kernels1x1)
    {
        delete m_kernels1x1;
        delete m_kernels1x1Bias;
    }
}


void Layer_Job::createLayerIters()
{
    m_num_krnl_iter = ceil((double)m_num3x3Kernels / (double)QUAD_MAX_KERNELS);
    m_num_depth_iter = ceil((double)m_inputMapDepth / (double)QUAD_DPTH_SIMD);
    int remNumKrnl = m_num3x3Kernels;
    int numKrnl;
    layAclPrm_t* layAclPrm;
    m_lay_it_arr.resize(m_num_krnl_iter);
    for (int i = 0, krnlBgn = 0; i < m_num_krnl_iter; i++, krnlBgn += numKrnl)
    {
        numKrnl = min(remNumKrnl, QUAD_MAX_KERNELS);
        int remDepth = m_inputMapDepth;
        int depth;
        for (int j = 0, depthBgn = 0; j < m_num_depth_iter; j++, depthBgn += depth)
        {
            depth = min(QUAD_DPTH_SIMD, remDepth);
            layAclPrm = createAccelParams(
                i,
                j,
                depthBgn,
                depth,
                krnlBgn,
                numKrnl
            );
            m_lay_it_arr[i].push_back(new Layer_Iteration(
                (j == 0) ? true : false,
                layAclPrm->inputMaps,
                layAclPrm->kernels3x3,
                layAclPrm->kernels1x1,
                layAclPrm->kernels3x3Bias,
                layAclPrm->kernels1x1Bias,
                layAclPrm->partialMaps,
                layAclPrm->residualMaps,
                layAclPrm->outputMaps,
                m_stride,
                m_upsample,
                m_padding,
                m_do_kernels1x1,
                (j == 0 && m_do_res_layer) ? true : false,
                m_activation
            ));
            remDepth -= depth;
        }
        remNumKrnl -= numKrnl;
    }
}


layAclPrm_t* Layer_Job::createAccelParams(
    int i,
    int j,
    int depthBgn,
    int depth,
    int krnlBgn,
    int numKrnl
) {
    layAclPrm_t* layAclPrm = new layAclPrm_t;
    memset(layAclPrm, 0x0, sizeof(layAclPrm_t));
    layAclPrm->inputMaps = m_inputMaps->GetVolume(depthBgn, depth);
    layAclPrm->kernels3x3 = m_kernels3x3->GetVolume(krnlBgn, numKrnl, depthBgn, depth);
    layAclPrm->kernels3x3Bias = m_kernels3x3Bias->GetVolume(krnlBgn, numKrnl);
    if(m_do_kernels1x1)
    {
        layAclPrm->kernels1x1 = m_kernels1x1->GetVolume(0, m_kernels1x1->m_numKernels, 0, m_kernels1x1->m_kernelDepth);
        layAclPrm->kernels1x1Bias = m_kernels1x1Bias->GetVolume(0, m_kernels1x1Bias->m_numKernels);
        layAclPrm->outputMaps = m_outputMaps = new OutputMaps(m_kernels1x1->m_numKernels, m_numOutputMapRows, m_numOutputMapCols);
    }
    else
    {
        layAclPrm->outputMaps = m_outputMaps = new OutputMaps(m_outputMapDepth, m_numOutputMapRows, m_numOutputMapCols);
    }
    if(j == 0)
    {
        layAclPrm->partialMaps = nullptr;
    }
    else
    {
        layAclPrm->partialMaps = m_lay_it_arr[i][j - 1]->m_outputMaps;
    }
    if(j == 0 && m_do_res_layer)
    {
        layAclPrm->residualMaps = m_residualMaps->GetVolume(krnlBgn, numKrnl);
    }
    return layAclPrm;
}


void Layer_Job::process()
{
    // Get configuration
    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        for (int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
			// printConfig(k, d);
            m_sysc_fpga_hndl->setConfig(m_lay_it_arr[k][d]->m_accelCfg);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_inputMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_kernels3x3);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_kernels1x1);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_partialMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_residualMaps);
            // m_sysc_fpga_hndl->setParam(m_lay_it_arr[k][d]->m_outputMaps);
            m_sysc_fpga_hndl->sendStart();
            m_sysc_fpga_hndl->waitComplete();
        }
    }
}


void Layer_Job::printConfig(int k, int d)
{
    ofstream fd;
    fd.open(string("./layer_cfg/") + m_layerName + string(".txt"));
    fd << m_layerName << endl;
    for(int k = 0; k < m_lay_it_arr.size(); k++)
    {
        fd << "\tKernel Iteration: " << k << endl;
        for (int d = 0; d < m_lay_it_arr[k].size(); d++)
        {
            fd << "\t\tDepth Iteration: " << d << endl;
            auto& FAS_cfg_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr;
            for (int a = 0; a < MAX_AWP_PER_FAS; a++)
            {
                auto& QUAD_en_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_en_arr;
                auto& QUAD_cfg_arr = m_lay_it_arr[k][d]->m_accelCfg->m_FAS_cfg_arr[0]->m_AWP_cfg_arr[a]->m_QUAD_cfg_arr;
                for (int q = 0; q < NUM_TOTAL_QUADS; q++)
                {
                    fd << "\t\t\tQUAD_id: "                 << QUAD_cfg_arr[q]->m_QUAD_id                 << endl;
                    if (QUAD_en_arr[q])
                    {
                        fd << "\t\t\t\tFAS_id:                  " << QUAD_cfg_arr[q]->m_FAS_id                  << endl;
                        fd << "\t\t\t\tAWP_id:                  " << QUAD_cfg_arr[q]->m_AWP_id                  << endl;
                        fd << "\t\t\t\tresidual:                " << FAS_cfg_arr[0]->m_do_res_layer             << endl;
                        fd << "\t\t\t\tfirst_depth_iter:        " << FAS_cfg_arr[0]->m_first_depth_iter         << endl;
                        fd << "\t\t\t\tkernel_1x1:              " << FAS_cfg_arr[0]->m_do_kernels1x1            << endl;
                        fd << "\t\t\t\tstride:                  " << QUAD_cfg_arr[q]->m_stride                  << endl;
                        fd << "\t\t\t\tnum_expd_input_rows:     " << QUAD_cfg_arr[q]->m_num_expd_input_rows     << endl;
                        fd << "\t\t\t\tnum_expd_input_cols:     " << QUAD_cfg_arr[q]->m_num_expd_input_cols     << endl;
                        fd << "\t\t\t\tcrpd_input_row_start:    " << QUAD_cfg_arr[q]->m_crpd_input_row_start    << endl;
                        fd << "\t\t\t\tcrpd_input_col_start:    " << QUAD_cfg_arr[q]->m_crpd_input_col_start    << endl;
                        fd << "\t\t\t\tcrpd_input_row_end:      " << QUAD_cfg_arr[q]->m_crpd_input_row_end      << endl;
                        fd << "\t\t\t\tcrpd_input_col_end:      " << QUAD_cfg_arr[q]->m_crpd_input_col_end      << endl;
                        fd << "\t\t\t\tnum_output_rows:         " << QUAD_cfg_arr[q]->m_num_output_rows         << endl;
                        fd << "\t\t\t\tnum_output_col:          " << QUAD_cfg_arr[q]->m_num_output_cols         << endl;
                        fd << "\t\t\t\tnum_kernels:             " << QUAD_cfg_arr[q]->m_num_kernels             << endl;
                        fd << "\t\t\t\tmaster_QUAD:             " << QUAD_cfg_arr[q]->m_master_QUAD             << endl;
                        fd << "\t\t\t\tcascade:                 " << QUAD_cfg_arr[q]->m_cascade                 << endl;
                        fd << "\t\t\t\tactivation:              " << QUAD_cfg_arr[q]->m_activation              << endl;
                        fd << "\t\t\t\tpadding:                 " << QUAD_cfg_arr[q]->m_padding                 << endl;
                        fd << "\t\t\t\tupsample:                " << QUAD_cfg_arr[q]->m_upsample                << endl;
                    }
                    else
                    {
                        fd << "\t\t\t\tUnused" << endl;
                    }
                }
            }
        }
    }
    fd.close();
}

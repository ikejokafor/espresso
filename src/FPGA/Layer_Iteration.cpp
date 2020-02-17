#include "Layer_Iteration.hpp"
using namespace std;


Layer_Iteration::Layer_Iteration(InputMaps* inputMaps, Kernels* kernels, int stride, bool upsample, int padding, bool conv_out_fmt0, bool residual, bool activation)
{
	int remDepth = inputMaps->m_inputMapDepth;
	for(int i = 0; i < NUM_FAS; i++)
	{
		m_fas_cfg_arr.push_back(new FAS_cfg(i, residual));
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			m_fas_cfg_arr[i]->m_awp_cfg_arr.push_back(new AWP_cfg(i ,j));
			for (int k = 0; k < NUM_QUAD_PER_AWP; k++)
			{
				if (remDepth > 0)
				{
					bool master_quad = (NUM_QUAD_PER_AWP  == 1) ? true
									   	: (NUM_QUAD_PER_AWP > 1 && k == (NUM_QUAD_PER_AWP - 1)) ? true : false;
					bool cascade = (NUM_QUAD_PER_AWP == 1) ? false
										: (NUM_QUAD_PER_AWP > 1 && k != (NUM_QUAD_PER_AWP - 1)) ? true : false;
					QUAD_cfg* quad_cfg = new QUAD_cfg(
						i,
						j,
						k,
						inputMaps->m_numInputMapRows, 
						inputMaps->m_numInputMapCols,
						kernels->m_numKernels,
						kernels->m_kernelDpeth,
						kernels->m_numKernelRows,
						kernels->m_numKernelCols,
						stride,
						upsample,
						padding,
						conv_out_fmt0,
						activation,
						master_quad,
						cascade
					);				
					m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_AWP_QUAD_cfg_arr.push_back(quad_cfg);
					m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_AWP_QUAD_cfg_en_arr.push_back(true);
				}
				else
				{
					m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_AWP_QUAD_cfg_en_arr.push_back(false);
				}
				remDepth -= QUAD_DPTH_SIMD;
			}
		}
	}
	
	
	for (int i = 0; i < NUM_FAS; i++)
	{
		cout << "FAS " << i << endl;
		for (int j = 0; j < MAX_AWP_PER_FAS; j++)
		{
			cout << "AWP " << j << endl;
			for (int k = 0; k < NUM_QUAD_PER_AWP; k++)
			{
				cout << "QUAD " << k << endl;
				auto& m_AWP_QUAD_cfg_arr = m_fas_cfg_arr[i]->m_awp_cfg_arr[j]->m_AWP_QUAD_cfg_arr;
				cout << "FAS_id: "                  << m_AWP_QUAD_cfg_arr[k]->m_FAS_id                  << endl;
				cout << "AWP_id: "                  << m_AWP_QUAD_cfg_arr[k]->m_AWP_id                  << endl;
				cout << "QUAD_id: "                 << m_AWP_QUAD_cfg_arr[k]->m_QUAD_id                 << endl;
				cout << "stride: "                  << m_AWP_QUAD_cfg_arr[k]->m_stride                  << endl;
				cout << "num_output_rows: "         << m_AWP_QUAD_cfg_arr[k]->m_num_output_rows         << endl;
				cout << "num_output_col: "          << m_AWP_QUAD_cfg_arr[k]->m_num_output_col          << endl;
				cout << "num_kernels: "             << m_AWP_QUAD_cfg_arr[k]->m_num_kernels             << endl;
				cout << "master_quad: "             << m_AWP_QUAD_cfg_arr[k]->m_master_quad             << endl;
				cout << "cascade: "                 << m_AWP_QUAD_cfg_arr[k]->m_cascade                 << endl;
				cout << "num_expd_input_rows: "     << m_AWP_QUAD_cfg_arr[k]->m_num_expd_input_rows     << endl;
				cout << "num_expd_input_cols: "     << m_AWP_QUAD_cfg_arr[k]->m_num_expd_input_cols     << endl;
				cout << "conv_out_fmt0: "           << m_AWP_QUAD_cfg_arr[k]->m_conv_out_fmt0           << endl;
				cout << "activation: "              << m_AWP_QUAD_cfg_arr[k]->m_activation              << endl;
				cout << "padding: "                 << m_AWP_QUAD_cfg_arr[k]->m_padding                 << endl;
				cout << "upsample: "                << m_AWP_QUAD_cfg_arr[k]->m_upsample                << endl;
				cout << "crpd_input_row_start: "    << m_AWP_QUAD_cfg_arr[k]->m_crpd_input_row_start    << endl;
				cout << "crpd_input_col_start: "    << m_AWP_QUAD_cfg_arr[k]->m_crpd_input_col_start    << endl;
				cout << "crpd_input_row_end: "      << m_AWP_QUAD_cfg_arr[k]->m_crpd_input_row_end      << endl;
				cout << "crpd_input_col_end: "      << m_AWP_QUAD_cfg_arr[k]->m_crpd_input_col_end      << endl;
			}
		}
	}
	cout << endl;
	cout << endl;
}


Layer_Iteration::~Layer_Iteration()
{

}
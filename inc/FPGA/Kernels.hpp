#pragma once


#include <vector>
#include <string.h>
#include "fixedPoint.hpp"
#include "util.hpp"
#include "FPGA_shim.hpp"
#ifdef SYSTEMC
#include "SYSC_FPGA_shim.hpp"
#else

#endif
#include "espresso_FPGA_common.hpp"


typedef std::vector<std::vector<float*>> krnl_data_t;


class Kernels : public Accel_Payload
{
	public:
		Kernels(FPGA_hndl* fpga_hndl, int numKernels, int depth, int rows, int cols, float* data);
		Kernels(FPGA_hndl* fpga_hndl, int numKernels, int depth, int rows, int cols, krnl_data_t& data);
        Kernels(Kernels* kernels);
        
        ~Kernels();
		void serialize();
        void deserialize();
		Kernels* GetVolume(int krnlBgn, int numKrnl, int depthBgn, int depthSize);


		int m_numKernels;
		int m_depth;
		int m_rows;
		int m_cols;
        FPGA_hndl*  m_fpga_hndl;
		krnl_data_t m_cpu_data;
};

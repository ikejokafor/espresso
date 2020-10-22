#include "KernelBias.hpp"
using namespace std;


KernelBias::KernelBias(FPGA_hndl* fpga_hndl, int numKernels, float* data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
    m_cpu_data      = data;
}


KernelBias::~KernelBias()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);
#else
    
#endif
}


void KernelBias::serialize()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_numKernels * PIXEL_SIZE;
    fixedPoint_t* rmt_data          = (fixedPoint_t*)sysc_fpga_hndl->allocate(this, m_size);

    for(int n = 0; n < m_numKernels; n++)
    {
        rmt_data[n] = fixedPoint::create(16, 14, m_cpu_data[n]);    // FIXME: remove hardcoding
    }
	
	m_buffer = (void*)rmt_data;
#else

#endif
}


void KernelBias::deserialize()
{

}


KernelBias* KernelBias::GetVolume(int krnlBgn, int numKrnl)
{
	float* ptr;
#ifdef FPGA
	ptr = (float*)(m_cpu_data + krnlBgn);
#endif
	return new KernelBias(m_fpga_hndl, numKrnl, ptr);
}

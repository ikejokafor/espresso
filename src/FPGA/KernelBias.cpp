#include "KernelBias.hpp"
using namespace std;


KernelBias::KernelBias(FPGA_hndl* fpga_hndl, int numKernels, float* data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
    m_cpu_data      = data;
	m_buffer		= NULL;
	m_size          = 0;
	m_remAddress    = -1;
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
    m_size                          = m_numKernels * sizeof(fixedPoint_t);
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

    for(int n = 0; n < m_numKernels; n++)
    {
        rmt_data[n] = fixedPoint::create(16, 14, m_cpu_data[n]);    // FIXME: remove hardcoding
    }
#else

#endif
}


void KernelBias::deserialize()
{

}


KernelBias* KernelBias::GetVolume(int krnlBgn, int numKrnl)
{
	float* ptr;
	ptr = (float*)(m_cpu_data + krnlBgn);
	return new KernelBias(m_fpga_hndl, numKrnl, ptr);
}

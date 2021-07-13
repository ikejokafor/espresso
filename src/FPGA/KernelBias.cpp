#include "KernelBias.hpp"
using namespace std;


KernelBias::KernelBias(FPGA_hndl* fpga_hndl, int numKernels, int krnlSize, float* data) : Accel_Payload()
{
    m_fpga_hndl     = fpga_hndl;
	m_numKernels    = numKernels;
    m_krnlSize      = krnlSize;
    m_cpu_data      = new float[numKernels];
    memcpy(m_cpu_data, data, sizeof(float) * numKernels);
	m_buffer		= NULL;
	m_size          = 0;
	m_remAddress    = -1;
}


KernelBias::~KernelBias()
{
#ifdef ALPHA_DATA
    fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	fpga_hndl->deallocate(this);   
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);    
#endif
}


void KernelBias::serialize()
{

#ifdef ALPHA_DATA
    fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_numKernels * sizeof(float);
    string ks = (m_krnlSize == 1) ? "1" : "3";
    printf("[ESPRESSO]: Allocating Space for %sx%s Kernel Biases\n", ks.c_str(), ks.c_str());    
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

    for(int n = 0; n < m_numKernels; n++)
    {
        rmt_data[n] = fixedPoint::create(16, 14, m_cpu_data[n]);    // FIXME: remove hardcoding
    }
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_numKernels * sizeof(float);
    string ks = (m_krnlSize == 1) ? "1" : "3";
    printf("[ESPRESSO]: Allocating Space for %sx%s Kernel Biases\n", ks.c_str(), ks.c_str());
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    float* rmt_data                 = (float*)m_buffer;

    for(int n = 0; n < m_numKernels; n++)
    {
        rmt_data[n] = m_cpu_data[n];    // FIXME: remove hardcoding
    }
#endif
}


void KernelBias::deserialize()
{

}


KernelBias* KernelBias::GetVolume(int krnlBgn, int numKrnl)
{
	float* ptr;
	ptr = (float*)(m_cpu_data + krnlBgn);
	return new KernelBias(m_fpga_hndl, numKrnl, m_krnlSize, ptr);
}

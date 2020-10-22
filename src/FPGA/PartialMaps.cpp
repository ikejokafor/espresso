#include "PartialMaps.hpp"
using namespace std;


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, int partialMapDepth, int numPartialMapRows, int numPartialMapCols, float* data)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = partialMapDepth;
	m_numPartialMapRows = numPartialMapRows;
	m_numPartialMapCols = numPartialMapCols;
	m_cpu_data          = data;
    m_no_permute        = false;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, InputMaps* inputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = inputMaps->m_inputMapDepth;
	m_numPartialMapRows = inputMaps->m_numInputMapRows;
	m_numPartialMapCols = inputMaps->m_numInputMapCols;
	m_cpu_data          = inputMaps->m_cpu_data;
    m_no_permute        = false;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = outputMaps->m_outputMapDepth;
	m_numPartialMapRows = outputMaps->m_numOutputMapRows;
	m_numPartialMapCols = outputMaps->m_numOutputMapCols;
    m_no_permute        = true;
}


PartialMaps::~PartialMaps()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);
#else
    
#endif
}


void PartialMaps::serialize()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols * PIXEL_SIZE;
    if(m_no_permute) 
    {
        memcpy(m_buffer, m_cpu_data, m_size);
        return;
    }
    fixedPoint_t* rmt_data           = (fixedPoint_t*)sysc_fpga_hndl->allocate(this, m_size);
	
    for(int r = 0; r < m_numPartialMapRows; r++)
    {
        for(int c = 0; c < m_numPartialMapCols; c++)
        {
            for(int d = 0; d < m_partialMapDepth; d++)
            {
                int rIdx = index3D(m_numPartialMapCols, m_partialMapDepth, r, c, d);
                int cIdx = index3D(m_numPartialMapRows, m_numPartialMapCols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding
            }
        }
    }

	m_buffer = (void*)rmt_data;
#else
    
#endif

}


void PartialMaps::deserialize()
{

}

#include "ResidualMaps.hpp"
using namespace std;


ResidualMaps::ResidualMaps(FPGA_hndl* fpga_hndl, int residualMapDepth, int numResidualMapRows, int numResidualMapCols, float* data) : Accel_Payload()
{
	m_fpga_hndl				= fpga_hndl;
	m_residualMapDepth      = residualMapDepth;
	m_numResidualMapRows    = numResidualMapRows;
	m_numResidualMapCols    = numResidualMapCols;
	m_cpu_data              = data;
}


ResidualMaps::~ResidualMaps()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);
#else
    
#endif
}


void ResidualMaps::serialize()
{
#ifdef SYSTEMC
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = m_residualMapDepth * m_numResidualMapRows * m_numResidualMapCols * PIXEL_SIZE;
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

    for(int r = 0; r < m_numResidualMapRows; r++)
    {
        for(int c = 0; c < m_numResidualMapCols; c++)
        {
            for(int d = 0; d < m_residualMapDepth; d++)
            {
                int rIdx = index3D(m_numResidualMapCols, m_residualMapDepth, r, c, d);
                int cIdx = index3D(m_numResidualMapRows, m_numResidualMapCols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding
            }
        }
    }
#else

#endif
}


void ResidualMaps::deserialize()
{

}


ResidualMaps* ResidualMaps::GetVolume(int depthBgn, int depthSize)
{
    float* ptr = (float*)(m_cpu_data + (depthBgn * m_numResidualMapRows * m_numResidualMapCols));
	return new ResidualMaps(m_fpga_hndl, depthSize, m_numResidualMapRows, m_numResidualMapCols, ptr);
}

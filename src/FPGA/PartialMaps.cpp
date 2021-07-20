#include "PartialMaps.hpp"
using namespace std;


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, int partialMapDepth, int numPartialMapRows, int numPartialMapCols, float* data)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = partialMapDepth;
	m_numPartialMapRows = numPartialMapRows;
	m_numPartialMapCols = numPartialMapCols;
    m_cpu_data          = new float[partialMapDepth * numPartialMapRows * numPartialMapCols];
	memcpy(m_cpu_data, data, sizeof(float) * partialMapDepth * numPartialMapRows * numPartialMapCols);
    m_no_permute        = true;
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, InputMaps* inputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = inputMaps->m_inputMapDepth;
	m_numPartialMapRows = inputMaps->m_numInputMapRows;
	m_numPartialMapCols = inputMaps->m_numInputMapCols;
    m_cpu_data          = new float[m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols];
	memcpy(m_cpu_data, inputMaps->m_cpu_data, sizeof(float) * m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols);
    m_no_permute        = true;
}


PartialMaps::PartialMaps(FPGA_hndl* fpga_hndl, OutputMaps* outputMaps)
{
    m_fpga_hndl         = fpga_hndl;
	m_partialMapDepth   = outputMaps->m_outputMapDepth;
	m_numPartialMapRows = outputMaps->m_numOutputMapRows;
	m_numPartialMapCols = outputMaps->m_numOutputMapCols;
    m_cpu_data          = new float[m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols];
    m_no_permute        = false;
}


PartialMaps::~PartialMaps()
{
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);
    free(m_cpu_data);
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);
    free(m_cpu_data);
#endif
}


void PartialMaps::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _fpga_hndl  		    	= reinterpret_cast<_hndl*>(m_fpga_hndl);
    m_size                          = m_partialMapDepth * m_numPartialMapRows * m_numPartialMapCols * PIXEL_SIZE;
    printf("[ESPRESSO]: Allocating Space for Partial Maps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;
    if(m_no_permute) 
    {
        memcpy(m_buffer, m_cpu_data, m_size);
        return;
    }
	
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
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = ACCL_MAX_KRNLS * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * sizeof(float);
    printf("[ESPRESSO]: Allocating Space for Partial Maps\n");
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    float* rmt_data                 = (float*)m_buffer;


    for(int d = 0; d < m_partialMapDepth; d++)
    {
        for(int r = 0; r < m_numPartialMapRows; r++)
        {
            for(int c = 0; c < m_numPartialMapCols; c++)
            {
                int rIdx = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, d, r, c);
                int cIdx = index3D(m_numPartialMapRows, m_numPartialMapCols, d, r, c);
                rmt_data[rIdx] = m_cpu_data[cIdx];
            }
        }
    }
#endif

}


void PartialMaps::deserialize()
{

}

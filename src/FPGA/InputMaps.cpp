#include "InputMaps.hpp"
using namespace std;


InputMaps::InputMaps(FPGA_hndl* fpga_hndl, int inputMapDepth, int numInputMapRows, int numInputMapCols, float* data) : Accel_Payload()
{
    m_fpga_hndl         = fpga_hndl;
	m_inputMapDepth     = inputMapDepth;
	m_numInputMapRows   = numInputMapRows;
	m_numInputMapCols   = numInputMapCols;
	m_cpu_data          = data;
	m_buffer			= NULL;
	m_size              = 0;
	m_remAddress        = -1;
}


InputMaps::~InputMaps()
{
#ifdef ALPHA_DATA
    _hndl* _hndl = reinterpret_cast<_hndl*>(m_fpga_hndl);
	_hndl->deallocate(this);  
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
	sysc_fpga_hndl->deallocate(this);   
#endif
}


void InputMaps::serialize()
{
#ifdef ALPHA_DATA
    _hndl* _hndl                    = reinterpret_cast< hndl*>(m_fpga_hndl);
    m_size                          = m_inputMapDepth * m_numInputMapRows * m_numInputMapCols * sizeof(fixedPoint_t);
    printf("[ESPRESSO]: Allocating Space for InputMaps\n");
    m_buffer                        = (void*)_hndl->allocate(this, m_size);
    fixedPoint_t* rmt_data          = (fixedPoint_t*)m_buffer;

	for(int r = 0; r < m_numInputMapRows; r++)
	{
		for(int c = 0; c < m_numInputMapCols; c++)
		{
			for(int d = 0; d < m_inputMapDepth; d++)
			{
                int rIdx = index3D(m_numInputMapCols, m_inputMapDepth, r, c, d);
                int cIdx = index3D(m_numInputMapRows, m_numInputMapCols, d, r, c);
                rmt_data[rIdx] = fixedPoint::create(16, 14, m_cpu_data[cIdx]);    // FIXME: remove hardcoding and pack values
            }
        }
    }
#else
    SYSC_FPGA_hndl* sysc_fpga_hndl  = reinterpret_cast<SYSC_FPGA_hndl*>(m_fpga_hndl);
    m_size                          = QUAD_DPTH_SIMD * QUAD_MAX_INPUT_ROWS * QUAD_MAX_INPUT_COLS * sizeof(float);
    printf("[ESPRESSO]: Allocating Space for InputMaps\n");
    m_buffer                        = (void*)sysc_fpga_hndl->allocate(this, m_size);
    float* rmt_data                 = (float*)m_buffer;

    for(int d = 0; d < m_inputMapDepth; d++)
    {
        for(int r = 0; r < m_numInputMapRows; r++)
        {
            for(int c = 0; c < m_numInputMapCols; c++)
            {
                int rIdx = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, d, r, c);
                int cIdx = index3D(m_numInputMapRows, m_numInputMapCols, d, r, c);
                rmt_data[rIdx] = m_cpu_data[cIdx];
            }
        }
    }
    
    FILE *fd = fopen("./inputMaps_fpga.txt", "w");
    for(int d = 0; d < m_inputMapDepth; d++)
    {
        for(int r = 0; r < m_numInputMapRows; r++)
        {
            for(int c = 0; c < m_numInputMapCols; c++)
            {
                int idx = index3D(QUAD_MAX_INPUT_ROWS, QUAD_MAX_INPUT_COLS, d, r, c);
                fprintf(fd, "%f ", rmt_data[idx]);
            }
            fprintf(fd, "\n");
        }
        fprintf(fd, "\n\n\n");
    }
    fclose(fd);
#endif
}


void InputMaps::deserialize()
{

}


InputMaps* InputMaps::GetVolume(int depthBgn, int depthSize)
{
	float* ptr = (float*)(m_cpu_data + (depthBgn * m_numInputMapRows * m_numInputMapCols));
	return new InputMaps(m_fpga_hndl, depthSize, m_numInputMapRows, m_numInputMapCols, ptr);
}

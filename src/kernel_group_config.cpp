#include "kernel_group_config.h"
#include <iostream>
#include <iomanip>

kernel_group_config::kernel_group_config(int kernels, int channels, int filValues, int fxPtLen, int numFracBits)
{
	m_kernels = kernels;
	m_channels = channels;
	m_filValues = filValues;
	m_fxPtLen = fxPtLen;
	m_numFracBits = numFracBits;
	
	m_Data = new fixedPoint_t**[m_kernels];
	for (int a = 0; a < m_kernels; a++) {
		m_Data[a] = new fixedPoint_t*[m_channels];
		for (int b = 0; b < m_channels; b++) {
			m_Data[a][b] = new fixedPoint_t[m_filValues];
			for (int i = 0; i < m_filValues; i++) {
				m_Data[a][b][i] = 0;
			}
		}
	}
}


kernel_group_config::~kernel_group_config()
{
	for (int a = 0; a < m_kernels; a++) {
		for (int b = 0; b < m_channels; b++) {
			free(m_Data[a][b]);
		}
		free(m_Data[a]);
	}
	free(m_Data);
}


void kernel_group_config::set_data(int k, int c, int v, fixedPoint_t data)
{
	m_Data[k][c][v] = data;
}


fixedPoint_t kernel_group_config::get_data(int k, int c, int v)
{
	return m_Data[k][c][v];
}


uint8_t * kernel_group_config::get_bytes(int & length)
{
	length = m_kernels * m_channels * m_filValues * sizeof(fixedPoint_t);
	fixedPoint_t* bytes = new fixedPoint_t[length];
	
	for (int k = 0; k < m_kernels; k++) {
		for (int c = 0; c < m_channels; c++) {
			for (int v = 0; v < m_filValues; v++) {
				int i0 = index3D(
					m_filValues,
					m_channels,
					k,
					v,
					c
				);
				bytes[i0] = m_Data[k][c][v];
			}
		}
	}

	return (uint8_t*)bytes;
}


void kernel_group_config::print(std::ofstream *od)
{	

}

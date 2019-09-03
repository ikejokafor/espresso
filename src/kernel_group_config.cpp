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
	
	m_Data = new fixedPoint_t[m_kernels * m_channels * filValues];
}


kernel_group_config::~kernel_group_config()
{
	free(m_Data);
}


void kernel_group_config::set_data(int k, int c, int v, fixedPoint_t data)
{	
	int idx = index3D(
		m_filValues,
		m_channels,
		k,
		v,
		c
	);
	m_Data[idx] = data;
}


fixedPoint_t kernel_group_config::get_data(int k, int c, int v)
{
	int idx = index3D(
		m_filValues,
		m_channels,
		k,
		v,
		c
	);
	return m_Data[idx];
}


uint8_t * kernel_group_config::get_bytes(int & length)
{
	length = m_kernels * m_channels * m_filValues * WEIGHT_SIZE;
	uint64_t* bytes = new uint64_t[int(ceil(float(length) / float(sizeof(uint64_t))))];
	int numValues = m_kernels * m_channels * m_filValues;
	int nValPerGrp = sizeof(uint64_t) / WEIGHT_SIZE;
	numValues -=  (numValues % nValPerGrp);
	int remValues = numValues % nValPerGrp;
	int i = 0;
	uint64_t zero = 0;
	for (i = 0; i < numValues; i += nValPerGrp) 
	{		
		bytes[i] = m_Data[i] << 0
			| m_Data[i + 1] << 16
			| m_Data[i + 2] << 32
			| m_Data[i + 3] << 48;
	}
	switch (remValues)
	{
		case 1:
		{
			bytes[i] = m_Data[i] << 0
				| zero << 16
				| zero << 32
				| zero << 48;
			break;
		}
		case 2:
		{
			bytes[i] = m_Data[i] << 0
				| m_Data[i + 1] << 16
				| zero << 32
				| zero << 48;
			break;
		}
		case 3:
		{
			bytes[i] = m_Data[i] << 0
				| m_Data[i + 1] << 16
				| m_Data[i + 2] << 32
				| zero << 48;
			break;
		}
	};
	return (uint8_t*)bytes;
}


void kernel_group_config::print(std::ofstream *od)
{	

}

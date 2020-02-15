#include "kernel.h"
#include <iostream>

kernel::kernel(int channels, int height, int width, int fxPtLen, int numFracBits)
{
	m_channels = channels;
	m_height = height;
	m_width = width;
	m_fxPtLen = fxPtLen;
	m_numFracBits = numFracBits;
	

	m_Data = new fixedPoint_t**[m_channels];
	for (int d0 = 0; d0 < m_channels; d0++) {
		m_Data[d0] = new fixedPoint_t*[m_height];
		for (int d1 = 0; d1 < m_height; d1++) {
			m_Data[d0][d1] = new fixedPoint_t[m_width];
		}
	}
}

kernel::~kernel()
{
	for (int i = 0; i < m_channels; i++) {
		for (int j = 0; j < m_height; j++) {
			free(m_Data[i][j]);
		}
		free(m_Data[i]);
	}
	free(m_Data);
}


kuid kernel::get_uid()
{
	return m_UID;
}


int kernel::get_length(int dim)
{
	switch(dim) {
		case 0: {
			return m_channels;
		}
		case 1: {
			return m_height;
		}
		case 2: {
			return m_width;
		}

	}
}


void kernel::set_weight(int c, int h, int w, fixedPoint_t data)
{
	m_Data[c][h][w] = data;
}


fixedPoint_t kernel::get_weight(int c, int h, int w)
{
	return m_Data[c][h][w];
}


kernel * kernel::get_slice(int slice_start, int slice_end)
{
	// int nSlices = (slice_end - slice_start) + 1;
	// kernel* krnl = new kernel(m_Dims[0], m_Dims[1], nSlices, m_fxPtLen, m_numFracBits);
	// 
	// for (int d0 = 0; d0 < m_Dims[0]; d0++) {
	// 	for (int d1 = 0; d1 < m_Dims[1]; d1++) {
	// 		for (int d2 = 0; d2 < nSlices; d2++) {
	// 			krnl->set_weight(d0, d1, d2, get_weight(d0, d1, d2 + slice_start));
	// 		}
	// 	}
	// }
	// 
	// return krnl;
	std::cout << "Not Implemented" << std::endl;
	return NULL;
}


void kernel::print(std::ofstream *od)
{
	std::ostream &outStream = (od->is_open()) ? od[0] : std::cout;
	for (int d0 = 0; d0 < m_channels; d0++) {
		for (int d1 = 0; d1 < m_height; d1++) {
			for (int d2 = 0; d2 < m_width; d2++) {
				float val = fixedPoint::toFloat(m_numFracBits, get_weight(d0, d1, d2));
				outStream << val << " " ;
			}
			outStream << std::endl;
		}
		outStream << std::endl;
		outStream << std::endl;
	}
}

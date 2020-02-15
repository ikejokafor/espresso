#include "kernel_group.h"


kernel_group::kernel_group(int channels, int height, int width, int fxPtLength, int numFracBits)
{
	m_channels = channels;
	m_height = height;
	m_width = width;
	m_fxPtLength = fxPtLength;
	m_numFracBits = numFracBits;
}


kernel_group::~kernel_group()
{
	
}


void kernel_group::add_kernel(kernel * krnl)
{
	int idx = get_num_kernels();

	if (krnl->get_length(0) == m_channels && krnl->get_length(1) == m_height && krnl->get_length(2) == m_width)
		m_Kernels[idx] = krnl;
}


kernel * kernel_group::get_kernel(int idx)
{
	kernel* val = nullptr;

	std::map<int, kernel*>::iterator it;

	it = m_Kernels.find(idx);
	if (it != m_Kernels.end())
		val = (*it).second;

	return val;
}


kernel_group_config * kernel_group::get_config()
{
	int nKernels = get_num_kernels();
	int nChannels = get_kernel_depth();
	int nCols = get_kernel_width();
	int nRows = get_kernel_height();
	int idx1 = 0;
	int idx0;
	kernel_group_config* kgc = new kernel_group_config(nKernels, QUAD_MAX_KRNL_DEPTH, NUM_KERNEL_3x3_VAL, m_fxPtLength, m_numFracBits);
	
	for (int k = 0; k < nKernels; k++) 
	{
		kernel* kernel_i = get_kernel(k);
		for (int y = 0; y < nRows; y++) 
		{
			for (int x = 0; x < nCols; x++) 
			{
				for (int c = 0; c < nChannels; c++) 
				{
					idx0 = idx1 % NUM_KERNEL_3x3_VAL;
					kgc->set_data(k, c, idx0, kernel_i->get_weight(c, y, x));
				}
				// pad with zeros depth wise if depth is less than QUAD_MAX_KRNL_DEPTH
				for (int c = nChannels; c < QUAD_MAX_KRNL_DEPTH; c++)
				{
					idx0 = idx1 % NUM_KERNEL_3x3_VAL;
					kgc->set_data(k, c, idx0, 0);
				}
				idx1++;
			}
		}
		// append extra zeros in 3x3 window
		for(int c = 0; c < QUAD_MAX_KRNL_DEPTH; c++)
		{
			idx0 = idx1 % NUM_KERNEL_3x3_VAL;
			kgc->set_data(k, c, idx0, 0);
		}
		idx1++;
	}
	return kgc;
}


int kernel_group::get_num_kernels()
{
	return m_Kernels.size();
}


int kernel_group::get_kernel_width()
{
	return m_width;
}


int kernel_group::get_kernel_height()
{
	return m_height;
}


int kernel_group::get_kernel_depth()
{
	return m_channels;
}


kernel_group * kernel_group::get_slice(int start_idx, int end_idx)
{
	return nullptr;
}


uint8_t * kernel_group::get_bytes()
{
	return nullptr;
}

int kernel_group::get_size()
{
	return 0;
}


void kernel_group::print()
{

}


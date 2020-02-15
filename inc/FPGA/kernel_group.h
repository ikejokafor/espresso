#pragma once

#include <map>

// #include "adapt_net_byte_serializeable_object.h"
#include "kernel.h"
#include "kernel_group_config.h"



//Structure that holds a set of NxMxD kernel objects
// class kernel_group : public adapt_net_byte_serializeable_object
class kernel_group
{
	public:
		//Constructor specifying the shape of the kernels to be stored in this group
		kernel_group(int numChannels, int height, int width, int fxPtLength, int numFracBits);

		//Deconstructor
		~kernel_group();


		void add_kernel(kernel* krnl);
		kernel* get_kernel(int idx);
		kernel_group_config* get_config();
		int get_num_kernels();
		int get_kernel_width();
		int get_kernel_height();
		int get_kernel_depth();
		kernel_group* get_slice(int start_idx, int end_idx);

		virtual uint8_t* get_bytes();
		virtual int get_size();

		void print();
	private:
		std::map<int, kernel*> m_Kernels;
		int m_channels;	
		int m_height;
		int m_width;
		int m_fxPtLength;
		int m_numFracBits;
};
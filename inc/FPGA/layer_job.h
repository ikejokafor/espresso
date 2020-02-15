#pragma once

#include <vector>
#include <string>

#include "adapt_net_allocator.h"
#include "kernel.h"
#include "layer_iteration.h"
#include "map_descriptor.h"

class layer_job
{
public:
	//Constructor
	layer_job(std::string name);

	//Deconstructor
	~layer_job();

	//Set the awe resource allocator 
	void set_allocator(adapt_net_allocator* allocator);

	//Add a single map_descriptor to the collection
	void add_map_descriptor(map_descriptor* descriptor);

	//Add a kernel to this layer job
	void add_kernel(kernel* krn);

	//Return all layer iterations from the collection
	std::vector<layer_iteration*> get_layer_iterations();

	//Create the job structures
	void initialize();

	float get_latency_cycles();

	std::string get_name();

	void print();

private:
	//Add a single layer iteration to the collection
	void add_layer_iteration(layer_iteration* iteration);
	
	//Create a set of iterations for a given map_descriptor and kernels
	void create_job_model0(map_descriptor* descriptor, std::vector<kernel*> kernels);

	//Layer jobs have one or more layer iterations
	std::vector<layer_iteration*> m_LayerIterations;

	std::vector<map_descriptor*> m_MapDescriptors;

	std::vector<kernel*> m_Kernels;

	adapt_net_allocator* m_Allocator;

	float m_Latency;

	std::string m_Name;
};

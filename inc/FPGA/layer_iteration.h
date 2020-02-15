#pragma once

#include <tuple>
#include <vector>
#include "adapt_net_allocator.h"
#include "kernel.h"
#include "kernel_group.h"
#include "map_job.h"
#include "map_descriptor.h"

class layer_iteration
{
public:
	//Constructor
	layer_iteration(std::string name);

	layer_iteration(std::string name, std::vector<map_descriptor*> maps, std::map<int, std::vector<kernel*>> parallel_kernels);

	//Deconstructor
	~layer_iteration();

	//Add a single map job
	void add_map_job(map_job* job);

	//Return all map jobs from the collection
	std::vector<map_job*> get_map_jobs();

	float get_latency_cyles();

	std::string get_name();

	void print();

	void configure_map_jobs(adapt_net_allocator* allocator);
private:

	//Layer iterations have one or more map jobs to execute
	std::vector<map_job*> m_MapJobs;

	std::vector<map_descriptor*> m_Maps;

	std::map<int, std::vector<kernel*>> m_Kernels;

	float m_Latency; //Latency in cycles

	std::string m_Name;
};

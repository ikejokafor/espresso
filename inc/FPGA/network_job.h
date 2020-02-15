#pragma once

#include <fstream>
#include <vector>
#include "adapt_net_allocator.h"
#include "layer_job.h"
#include "map_job.h"



class network_job
{
public:
	//Constructor
	network_job();

	//Deconstructor
	~network_job();

	//Initialize all jobs
	void initialize();

	void initialize(std::ifstream& definition, int layer_start=0, int layer_end=0x7FFFFFFF);
	
	//Set the awe resource allocator 
	void set_allocator(adapt_net_allocator* allocator);

	//Get the next set of jobs to perform on the network
	std::vector<map_job*> get_next_jobs();

	float get_latency_cycles();

	void print();

	void set_clock_rate(float rate);

private:
	//Add a single layer job to the collection
	void add_layer_job(layer_job* job);

	//Return all layer jobs from the collection
	std::vector<layer_job*> get_layer_jobs();

	//Network jobs have one or more layer jobs that execute serially 
	std::vector<layer_job*> m_LayerJobs;

	int m_IdxLayerJob;
	int m_IdxLayerIteration;

	float m_ClockRate;

	adapt_net_allocator* m_Allocator;
	
};
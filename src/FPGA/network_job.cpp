
#include <string>
#include <sstream>
#include <iostream>
#include "network_job.h"

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(trim(token));
	}
	return tokens;
}

bool get_layer_parameters(std::string parameters, int& map_width, int& map_height, int& map_depth, int& kernel_count, int& kernel_width, int& kernel_height, int& kernel_depth)
{
	std::vector<std::string>split_parameters = split(parameters, ',');
	if (split_parameters.size() != 2)
		return false;

	std::vector<std::string>map_parameters = split(split_parameters[0], 'x');
	if (map_parameters.size() != 3)
		return false;

	std::string::size_type sz;   // alias of size_t

	map_width = std::stoi(map_parameters[0], &sz);
	map_height = std::stoi(map_parameters[1], &sz);
	map_depth = std::stoi(map_parameters[2], &sz);

	std::vector<std::string>kernel_parameters = split(split_parameters[1], 'x');
	if (kernel_parameters.size() != 4)
		return false;

	kernel_count = std::stoi(kernel_parameters[0], &sz);
	kernel_width = std::stoi(kernel_parameters[1], &sz);
	kernel_height = std::stoi(kernel_parameters[2], &sz);
	kernel_depth = std::stoi(kernel_parameters[3], &sz);

	return true;
}

network_job::network_job()
{
	set_clock_rate(100000000.);
}

network_job::~network_job()
{
}

void network_job::set_clock_rate(float rate)
{
	m_ClockRate = rate;
}

void network_job::set_allocator(adapt_net_allocator* allocator)
{
	m_Allocator = allocator;
}

void network_job::initialize()
{
	m_IdxLayerJob = 0;
	m_IdxLayerIteration = 0;
}

void network_job::initialize(std::ifstream& definition, int layer_start, int layer_end)
{
	m_IdxLayerJob = 0;
	m_IdxLayerIteration = 0;

	m_LayerJobs.clear();
	int layerIdx = 0;

	std::string line;
	while (std::getline(definition, line))
	{
		if (line == "")
			continue;

		if (layerIdx >= layer_start && layerIdx <= layer_end) {

			int kernel_count = 0;
			int kernel_width = 0;
			int kernel_height = 0;
			int kernel_depth = 0;
			int map_width = 0;
			int map_height = 0;
			int map_depth = 0;

			get_layer_parameters(line, map_width, map_height, map_depth, kernel_count, kernel_width, kernel_height, kernel_depth);

			std::string layer_name = "Layer-" + std::to_string(layerIdx);

			layer_job* job = new layer_job(layer_name);

			job->set_allocator(m_Allocator);
			job->add_map_descriptor(new map_descriptor(map_width, map_height, map_depth));

			for (int k = 0; k < kernel_count; k++)
				job->add_kernel(new kernel(kernel_width, kernel_height, map_depth));

			job->initialize();

			add_layer_job(job);
		}
		layerIdx++;
	}
}

//Iterate, on each call, through the map_jobs that have been created for the network.
std::vector<map_job*> network_job::get_next_jobs()
{
	std::vector<map_job*> m_jobs;

	std::vector<layer_job*> layer_jobs = get_layer_jobs();

	for (; m_IdxLayerJob < layer_jobs.size(); m_IdxLayerJob++) {
		std::vector<layer_iteration*> layer_iterations = layer_jobs[m_IdxLayerJob]->get_layer_iterations();
		for (; m_IdxLayerIteration < layer_iterations.size(); m_IdxLayerIteration++) {
			m_jobs = layer_iterations[m_IdxLayerIteration]->get_map_jobs();

			if (m_jobs.size() != 0)
				break;
		}
	}
	
	return m_jobs;
}

void network_job::add_layer_job(layer_job * job)
{
	m_LayerJobs.push_back(job);
}

std::vector<layer_job*> network_job::get_layer_jobs()
{
	return m_LayerJobs;
}

float network_job::get_latency_cycles()
{
	float latency = 0.;

	std::vector<layer_job*> jobs = get_layer_jobs();

	for (int i = 0; i < jobs.size(); i++)
		latency += jobs[i]->get_latency_cycles();

	return latency;
}

void network_job::print()
{
	std::cout << "------------------------------------------------------------" << std::endl;

	std::vector<layer_job*> jobs = get_layer_jobs();

	for (int i = 0; i < jobs.size(); i++)
		jobs[i]->print();

	float latency = get_latency_cycles() / m_ClockRate;

	std::cout << "-------------------- Latency " << latency << " seconds : " << 1./latency << " FPS ----------------------------" << std::endl;
}
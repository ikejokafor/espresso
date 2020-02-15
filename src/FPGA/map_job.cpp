#include "map_job.h"
#include <iostream>

map_job* map_job::create(std::string identifier, map_descriptor* map, std::vector<kernel*> kernels, adapt_net_allocator* allocator, fas_aggregation_context* aggregation_resource)
{
	map_job* job = nullptr;

	fas_aggregation_context* aggregation_context = nullptr;
	fas_store_context* store_context = nullptr;
	std::vector<awe_cluster_cascade*> cascades;

	if ((cascades = allocator->reserve_awe_cluster_cascades(map, kernels)).size() != 0) {
		if ((store_context = allocator->reserve_fas_store_context()) != nullptr) {
			if (cascades.size() > 1) {
				//If multiple cascades are involved, we need an aggregation context to combine them
				if ((aggregation_context = allocator->reserve_fas_aggregation_context(store_context->get_id())) != nullptr) {
					aggregation_context->assign_awe_cascades(cascades);
					
					//Assign the aggregation context as the source of the store context
					aggregation_context->connect(store_context);
				}
				else {
					//Couldnt get an aggregation context so release any resources reserved so far
					allocator->release_fas_store_context(store_context);
					allocator->release_awe_cluster_cascades(cascades);
				}
			}
			else {
				//There is only one cascade so no aggregation is necessary and the map source is set to the only cascade
				cascades.front()->connect(store_context);
			}
			job = new map_job(identifier, store_context, aggregation_context, cascades);
		}
	}

	return job;
}

map_job::map_job(std::string identifier, fas_store_context* store_context, fas_aggregation_context* aggregation_context, std::vector<awe_cluster_cascade*> cascades)
{
	m_Identifier = identifier;
	m_FASStoreContext = store_context;
	m_AggregationContext = aggregation_context;
	m_AWEClusterCascades = cascades;
}



map_job::~map_job()
{
}

void map_job::add_awp_configuration(awp_configuration * config)
{
}

std::vector<awp_configuration*> map_job::get_awp_configurations()
{
	return std::vector<awp_configuration*>();
}

/*
void map_job::add_fas_configuration(fas_configuration * config)
{
}

std::vector<fas_configuration*> map_job::get_fas_configurations()
{
	return std::vector<fas_configuration*>();
}
*/

void map_job::print()
{
	std::cout << "---------------------------------------------------------------" << std::endl;	
	std::vector<adapt_net_component*> t;
	t.push_back(m_FASStoreContext);

	do_print(t);
	//do_print_map_job_info();
	//do_print_store_context_info();
	//do_print_aggregation_context_info();
	//do_print_cascade_info();
	//do_print_fetch_context_info();
	std::cout << "---------------------------------------------------------------" << std::endl;
}

std::string map_job::get_identifier()
{
	return m_Identifier;
}

std::vector<adapt_net_configuration_list*> map_job::get_configuration()
{
	std::vector<adapt_net_component*> t;
	std::vector<adapt_net_configuration_list*> config_list;

	t.push_back(m_FASStoreContext);

	do_get_configuration(t, config_list);

	return config_list;
}

std::vector<adapt_net_configuration_list*> map_job::get_execution_list()
{
	return std::vector<adapt_net_configuration_list*>();
}

void map_job::do_print_map_job_info()
{
	std::cout << "Map Job Info - " << get_identifier().c_str() << std::endl;
}

void map_job::do_print_store_context_info() 
{
	m_FASStoreContext->print();
}

void map_job::do_print_aggregation_context_info()
{
	if (m_AggregationContext != nullptr)
		m_AggregationContext->print();
}

void map_job::do_print_cascade_info()
{
	for (auto const& cascade : m_AWEClusterCascades) {
		cascade->print();
	}
}

void map_job::do_print_fetch_context_info()
{
	//auto current_sources = m_FASStoreContext->get_sources()
	for (auto const& cascade : m_AWEClusterCascades) {
		for (auto const& cluster : cascade->get_sources()) {
			for (auto const& fetch : cluster->get_sources()) {
				fetch->print();
			}
		}
	}
}

void map_job::do_print(std::vector<adapt_net_component*> sources)
{
	if (sources.size() > 0) {
		for (auto const& s : sources) {
			do_print(s->get_sources());

			s->print();
		}
	}
}

void map_job::do_get_configuration(std::vector<adapt_net_component*> sources, std::vector<adapt_net_configuration_list*>& config_list)
{
	if (sources.size() > 0) {
		for (auto const& s : sources) {
			do_get_configuration(s->get_sources(), config_list);
			auto configs = s->get_configuration();

			config_list.insert(config_list.end(),configs.begin(),configs.end());
		}
	}
}

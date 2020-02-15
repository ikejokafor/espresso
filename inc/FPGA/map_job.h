#pragma once

#include <vector>

#include "adapt_net_allocator.h"
#include "awe_cluster.h"
#include "awe_cluster_cascade.h"
#include "awp_configuration.h"
#include "fas_aggregation_context.h"
#include "fas_fetch_context.h"
#include "fas_store_context.h"
//#include "fas_configuration.h"
#include "kernel_group.h"
#include "map_descriptor.h"


class map_job 
{
public:
	//Static creator
	static map_job* create(std::string identifier, map_descriptor* map, std::vector<kernel*> kernels, adapt_net_allocator* allocator, fas_aggregation_context* aggregation_resource = nullptr);

	//Deconstructor
	~map_job();

	//Add a single awp_configuration to the collection
	void add_awp_configuration(awp_configuration* config);

	//Return all awp_configurations from the collection
	std::vector<awp_configuration*> get_awp_configurations();

	//Add a single fas_configuration to the collection
	//void add_fas_configuration(fas_configuration* config);

	//Return all fas_configurations from the collection
	//std::vector<fas_configuration*> get_fas_configurations();

	void print();

	std::string get_identifier();

	std::vector<adapt_net_configuration_list*> get_configuration();

	std::vector<adapt_net_configuration_list*> get_execution_list();

protected:
	virtual void do_print_map_job_info();
	virtual void do_print_store_context_info();
	virtual void do_print_aggregation_context_info();
	virtual void do_print_cascade_info();
	virtual void do_print_fetch_context_info();
private:
	//Constructor
	map_job(std::string identifier, fas_store_context* store_context, fas_aggregation_context* aggregation_context, std::vector<awe_cluster_cascade*> cascades);

	//A map iteration has one or more awp_configurations
	std::vector<awp_configuration*> m_AWPConfigurations;

	//A map iteration has one or more fas_configurations
	//std::vector<fas_configuration*> m_FASConfigurations;

	std::vector<fas_fetch_context*> m_FASFetchContexts;

	fas_store_context* m_FASStoreContext;

	fas_aggregation_context* m_AggregationContext;

	std::vector<awe_cluster_cascade*> m_AWEClusterCascades;

	std::string m_Identifier;

	void do_print(std::vector<adapt_net_component*> sources);
	void do_get_configuration(std::vector<adapt_net_component*> sources, std::vector<adapt_net_configuration_list*>& config_list);
};
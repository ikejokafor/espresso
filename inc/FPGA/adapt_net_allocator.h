#pragma once

#include <list>
#include <vector>

#include "awp.h"
#include "awe_cluster.h"
#include "awe_cluster_cascade.h"
#include "fas.h"
#include "fas_aggregation_context.h"
#include "fas_config_context.h"
#include "fas_fetch_context.h"
#include "fas_store_context.h"

class adapt_net_allocator
{
public:
	//Constructor
	adapt_net_allocator();

	//Deconstructor
	~adapt_net_allocator();

	//Initialize AWE allocator
	void initialize();

	//Add an awe object to the available resources
	void add_awp(awp* engine);

	//Add an fas object with a specific function
	void add_fas(fas* engine);

	std::vector<fas_config_context*> get_fas_config_contexts();

	std::vector<awe_cluster_cascade*> reserve_awe_cluster_cascades(map_descriptor* map, std::vector<kernel*> kernels);

	void release_awe_cluster_cascades(std::vector<awe_cluster_cascade*> cascades);

	//Reserve a fas fetch function
	fas_fetch_context* reserve_fas_fetch_context();

	//Release a fas fetch context
	void release_fas_fetch_context(fas_fetch_context* context);

	//Reserve a fas store function
	fas_store_context* reserve_fas_store_context();

	//Release a fas store function
	void release_fas_store_context(fas_store_context* context);

	//Reserve a fas aggregation function
	fas_aggregation_context* reserve_fas_aggregation_context(adapt_net_context_id id = { -1,-1 });

	//Get the SIMD value for the AWE with configured as kernel_width x kernel_width convolution
	int get_awe_cluster_simd(AWE_CLUSTER_MODE mode);

	int get_awe_cluster_map_dimension_minimum(int dim, AWE_CLUSTER_MODE mode);

	int get_awe_cluster_map_dimension_maximum(int dim, AWE_CLUSTER_MODE mode);

	//Get the number of available AWE
	int get_awe_cluster_count();

	//Get the number of available FAS Fetch function
	int get_fas_fetch_context_count();

	//Get the number of available FAS Aggregation function
	int get_fas_aggregation_context_count();


protected:
	//Reserve count number of awe clusters.  Returns awe_clusters grouped by parent AWP
	std::vector<awe_cluster*> reserve_awe_cluster(int count);
private:
	
	std::vector<fas*> m_FAS;
	std::vector<awp*> m_AWP;
	std::list<awe_cluster*> m_AvailableAWECluster;
	std::vector<fas_config_context*> m_AvailableFASConfigContext;
	std::list<fas_fetch_context*> m_AvailableFASFetchContext;
	std::list<fas_store_context*> m_AvailableFASStoreContext;
	std::list<fas_aggregation_context*> m_AvailableFASAggregationContext;

	std::vector<fas_fetch_context*> m_AllocatedFASFetchContext;
	std::vector<fas_store_context*> m_AllocatedFASStoreContext;
	std::vector<fas_aggregation_context*> m_AllocatedFASAggregationContext;
	 
};

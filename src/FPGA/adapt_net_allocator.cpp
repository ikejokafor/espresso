#include "adapt_net_allocator.h"

#include <iostream>

adapt_net_allocator::adapt_net_allocator()
{
}

adapt_net_allocator::~adapt_net_allocator()
{
	/*
	for (auto const& i : m_AllocatedFASAggregationContext) {
		delete(i);
	}
	*/
}

void adapt_net_allocator::initialize()
{
	m_AvailableAWECluster.clear();
	m_AvailableFASFetchContext.clear();
	m_AvailableFASStoreContext.clear();
	m_AvailableFASAggregationContext.clear();

	for (std::size_t i = 0; i < m_AWP.size(); ++i) {
		m_AWP[i]->initialize();
	}

	for (std::size_t i = 0; i < m_FAS.size(); ++i) {
		m_FAS[i]->initialize();
	}

	for (std::size_t i = 0; i < m_AWP.size(); ++i) {
		std::vector<awe_cluster*> clusters = m_AWP[i]->get_awe_clusters();
		m_AvailableAWECluster.insert(m_AvailableAWECluster.end(), clusters.begin(), clusters.end());
	}

	for (std::size_t i = 0; i < m_FAS.size(); ++i) {
		std::vector<fas_config_context*> contexts = m_FAS[i]->get_config_context();
		m_AvailableFASConfigContext.insert(m_AvailableFASConfigContext.end(), contexts.begin(), contexts.end());
	}

	for (std::size_t i = 0; i < m_FAS.size(); ++i) {
		std::vector<fas_fetch_context*> contexts = m_FAS[i]->get_fetch_context();
		m_AvailableFASFetchContext.insert(m_AvailableFASFetchContext.end(), contexts.begin(), contexts.end());
	}

	for (std::size_t i = 0; i < m_FAS.size(); ++i) {
		std::vector<fas_store_context*> contexts = m_FAS[i]->get_store_context();
		m_AvailableFASStoreContext.insert(m_AvailableFASStoreContext.end(), contexts.begin(), contexts.end());
	}

	for (std::size_t i = 0; i < m_FAS.size(); ++i) {
		std::vector<fas_aggregation_context*> contexts = m_FAS[i]->get_aggregation_context();
		m_AvailableFASAggregationContext.insert(m_AvailableFASAggregationContext.end(), contexts.begin(), contexts.end());
	}
}

void adapt_net_allocator::add_awp(awp * engine)
{
	m_AWP.push_back(engine);
}

void adapt_net_allocator::add_fas(fas * engine)
{
	m_FAS.push_back(engine);
}

std::vector<fas_config_context*> adapt_net_allocator::get_fas_config_contexts()
{
	return m_AvailableFASConfigContext;
}


std::vector<awe_cluster*> adapt_net_allocator::reserve_awe_cluster(int count)
{	
	std::vector<awe_cluster*> clusters;

	if (m_AvailableAWECluster.size() < count)
		return clusters;

	for (int i = 0; i < count; i++) {
		clusters.push_back(m_AvailableAWECluster.front());
		m_AvailableAWECluster.pop_front();
	}
	
	return clusters;
}

std::vector<awe_cluster_cascade*> adapt_net_allocator::reserve_awe_cluster_cascades(map_descriptor* map, std::vector<kernel*> kernels)
{
	
	int kernel_width = kernels[0]->get_length(0);
	int kernel_height = kernels[0]->get_length(1);
	int kernel_depth = kernels[0]->get_length(2);

	int awe_simd = get_awe_cluster_simd((AWE_CLUSTER_MODE)kernel_width);
	int num_awe_clusters_required = std::ceil((double)kernel_depth / (double)awe_simd);

	int kernel_group_channels = std::fmin(awe_simd, kernel_depth);

	std::vector<awe_cluster*> clusters = reserve_awe_cluster(num_awe_clusters_required);
	for (int i = 0; i < clusters.size(); i++) {
		int slice_idx_start = i * awe_simd;
		int slice_idx_end = std::fmin(slice_idx_start + awe_simd - 1, kernel_depth - 1);

		map_descriptor* map_slice = map->get_volume(0, map->get_length(0) - 1, 0, map->get_length(1) - 1, slice_idx_start, slice_idx_end);
		
		fas_fetch_context* fc = reserve_fas_fetch_context();
		fc->set_map_descriptor(map_slice);

		std::cout << "Num available fc " << m_AvailableFASFetchContext.size() << std::endl;

		kernel_group* kg = new kernel_group(kernel_height, kernel_width, kernel_group_channels);
		for (int k = 0; k < kernels.size(); k++)
			kg->add_kernel(kernels[k]->get_slice(slice_idx_start, slice_idx_end));

		//Connect the Fetch Context as the map source for the current cluster
		fc->connect(clusters[i]);
		clusters[i]->set_kernel_group(kg);
		
		//Probably not necessary as map_slice can be observed through fetch context
		clusters[i]->set_map(map_slice);
	}
	
	return awe_cluster_cascade::create_cascades(clusters);
}

void adapt_net_allocator::release_awe_cluster_cascades(std::vector<awe_cluster_cascade*> cascades)
{
}

fas_fetch_context * adapt_net_allocator::reserve_fas_fetch_context()
{
	if (m_AvailableFASFetchContext.size() == 0)
		return nullptr;

	fas_fetch_context* context = m_AvailableFASFetchContext.front();
	m_AvailableFASFetchContext.pop_front();

	return context;
}

void adapt_net_allocator::release_fas_fetch_context(fas_fetch_context * context)
{
}

fas_aggregation_context * adapt_net_allocator::reserve_fas_aggregation_context(adapt_net_context_id id)
{
	fas_aggregation_context* context = nullptr;

	if (id.unit_id != -1 && id.context_id != -1) {
		//Find the aggregation context matching unit_id and context_id
		for (auto const& x : m_AvailableFASAggregationContext) {
			auto c_id = x->get_id();
			if ((c_id.unit_id == id.unit_id) && (c_id.context_id == id.context_id)) {
				context = x;
				break;
			}
		}

		if (context != nullptr)
			m_AvailableFASAggregationContext.remove(context);
	}
	else
	{
		if (m_AvailableFASAggregationContext.size() == 0)
			return nullptr;

		context = m_AvailableFASAggregationContext.front();
		m_AvailableFASAggregationContext.pop_front();
	}

	return context;
}

fas_store_context * adapt_net_allocator::reserve_fas_store_context()
{
	if (m_AvailableFASStoreContext.size() == 0)
		return nullptr;

	fas_store_context* context = m_AvailableFASStoreContext.front();
	m_AvailableFASStoreContext.pop_front();

	return context;
}

void adapt_net_allocator::release_fas_store_context(fas_store_context * context)
{
}

int adapt_net_allocator::get_awe_cluster_simd(AWE_CLUSTER_MODE mode)
{
	return awp::get_awe_cluster_simd(mode);
}

int adapt_net_allocator::get_awe_cluster_map_dimension_minimum(int dim, AWE_CLUSTER_MODE mode)
{
	return awp::get_awe_cluster_map_dimension_minimum(dim, mode);
}

int adapt_net_allocator::get_awe_cluster_map_dimension_maximum(int dim, AWE_CLUSTER_MODE mode)
{
	return awp::get_awe_cluster_map_dimension_maximum(dim, mode);
}

int adapt_net_allocator::get_awe_cluster_count()
{
	return m_AvailableAWECluster.size();
}

int adapt_net_allocator::get_fas_fetch_context_count()
{
	return m_AvailableFASFetchContext.size();
}

int adapt_net_allocator::get_fas_aggregation_context_count()
{
	return m_AvailableFASAggregationContext.size();
}

adapt_net_memory_reference adapt_net_allocator::allocate_memory(size_t size)
{
	return adapt_net_memory_reference();
}

#include "fas_aggregation_context.h"
#include "awe_cluster.h"
#include "awe_cluster_cascade.h"
#include <iostream>

fas_aggregation_context::fas_aggregation_context(adapt_net_context_id id)
	: adapt_net_component(ADAPT_NET_COMPONENT_TYPE::FAS_AGGREGATION_CONTEXT, id)
{
}

fas_aggregation_context::~fas_aggregation_context()
{
}

void fas_aggregation_context::assign_awe_cascades(std::vector<awe_cluster_cascade*> cascades)
{
	for (auto const& c : cascades) {
		auto cluster = c->get_egress_awe();
		cluster->set_cluster_fas_aggregate_context(this);
		m_SourceAWEClusters.push_back(cluster);
		m_Cascades.push_back(c);
		c->connect(this);
	}
}

void fas_aggregation_context::print()
{
	std::cout << "\tConfiguration - \tFAS Aggregation Context [" << get_id().unit_id << "," << get_id().context_id << "] digests " << m_Cascades.size() << " Cascades" << std::endl;
}

map_descriptor * fas_aggregation_context::get_map()
{
	return nullptr;
}

void fas_aggregation_context::do_print_cascades()
{
}

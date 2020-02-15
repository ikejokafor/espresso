#include "awe_cluster_cascade.h"

#include <algorithm>
#include <iostream>

awe_cluster_cascade::awe_cluster_cascade()
	:adapt_net_component(ADAPT_NET_COMPONENT_TYPE::AWE_CLUSTER_CASCADE, adapt_net_context_id(-1, -1))
{
}

awe_cluster_cascade::~awe_cluster_cascade()
{
}

// Compares two clusters according to cluster_id. 
bool compare_awe_cluster(awe_cluster* c0, awe_cluster* c1)
{
	return (c0->get_id().context_id < c1->get_id().context_id);
}

std::vector<awe_cluster_cascade*> awe_cluster_cascade::create_cascades(std::vector<awe_cluster*> clusters)
{
	std::vector<awe_cluster_cascade*> cluster_cascades;
	awe_cluster_cascade* current_cascade = nullptr;

	//Group by awp id
	std::map<int, std::vector<awe_cluster*> > clusters_by_awp;
	
	for (const auto & c : clusters) clusters_by_awp[c->get_id().unit_id].push_back(c);

	for (int a = 0; a < clusters_by_awp.size(); a++) {
		current_cascade = nullptr;
		//sort the clusters in a single awp
		std::sort(clusters_by_awp[a].begin(), clusters_by_awp[a].end(), compare_awe_cluster);

		for (int c = 0; c < clusters_by_awp[a].size(); c++) {
			if (current_cascade == nullptr) {
				current_cascade = new awe_cluster_cascade();
				current_cascade->add_cluster(clusters_by_awp[a][c]);
				cluster_cascades.push_back(current_cascade);
			}
			else if (clusters_by_awp[a][c]->get_id().context_id == (current_cascade->get_egress_awe()->get_id().context_id + 1)) {
				current_cascade->add_cluster(clusters_by_awp[a][c]);
			}
			else // same awp but gap between current cluster and previous: therefore a new cascade
			{
				current_cascade = new awe_cluster_cascade();
				current_cascade->add_cluster(clusters_by_awp[a][c]);
				cluster_cascades.push_back(current_cascade);
			}
		}
	}

	return cluster_cascades;
}

awe_cluster * awe_cluster_cascade::get_ingress_awe()
{
	return nullptr;
}

awe_cluster * awe_cluster_cascade::get_egress_awe()
{
	return m_Cascade.back();
}

int awe_cluster_cascade::get_cluster_count()
{
	return 0;
}

map_descriptor * awe_cluster_cascade::get_map()
{
	return nullptr;
}

std::vector<adapt_net_component*> awe_cluster_cascade::get_targets()
{
	std::vector<adapt_net_component*> targets;

	for (auto const& x : m_Cascade)
		targets.push_back(x);

	return targets;
}

std::vector<adapt_net_component*> awe_cluster_cascade::get_sources()
{
	std::vector<adapt_net_component*> sources;
	for (auto const& x : m_Cascade)
		sources.push_back(x);

	return sources;
}

void awe_cluster_cascade::print()
{	
	std::cout << "\tConfiguration - \tAWE Cluster Cascade containing " << m_Cascade.size() << " AWE Clusters" << std::endl;
	for (auto const& x : m_Cascade) {
		x->print();
		//std::cout << "\t\t\t\tAWE Cluster [" << x->get_id().unit_id << ", " << x->get_id().context_id << "]" << std::endl;
	}
}

void awe_cluster_cascade::add_cluster(awe_cluster * cluster)
{
	m_Cascade.push_back(cluster);
}

#pragma once
#include <list>
#include <vector>
#include "adapt_net_component.h"
#include "awe_cluster.h"

class awe_cluster_cascade : public adapt_net_component
{
public:
	awe_cluster_cascade();
	~awe_cluster_cascade();

	static std::vector<awe_cluster_cascade*> create_cascades(std::vector<awe_cluster*> clusters);

	awe_cluster * get_ingress_awe();
	awe_cluster * get_egress_awe();

	int get_cluster_count();

	map_descriptor* get_map();
	
	//void connect(adapt_net_map_target* target) { adapt_net_map_source::connect(target); /*target->add_map_source(this);*/ }
	//void connect(adapt_net_map_source* source) { adapt_net_map_target::connect(source); /*source->add_map_target(this);*/ }

	std::vector<adapt_net_component*> get_targets();
	std::vector<adapt_net_component*> get_sources();

	void print();
private:
	
	

	std::list<awe_cluster*> m_Cascade;

	void add_cluster(awe_cluster* cluster);
};

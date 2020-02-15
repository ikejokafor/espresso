#pragma once

#include <vector>
#include "adapt_net_component.h"
#include "map_descriptor.h"
//#include "awe_cluster.h"
//#include "awe_cluster_cascade.h"

class awe_cluster_cascade;
class awe_cluster;


class fas_aggregation_context : public adapt_net_component
{
public:
	fas_aggregation_context(adapt_net_context_id id);
	~fas_aggregation_context();

	void assign_awe_cascades(std::vector<awe_cluster_cascade*> cascades);
	void print();

	map_descriptor* get_map();

	//void connect(adapt_net_map_target* target) { adapt_net_map_source::connect(target); }
	//void connect(adapt_net_map_source* source) { adapt_net_map_target::connect(source); }

protected:
	virtual void do_print_cascades();
private:

	std::vector<awe_cluster_cascade*> m_Cascades;
	std::vector<awe_cluster*> m_SourceAWEClusters;
};

#pragma once

#include <kernel_group.h>
#include "adapt_net_component.h"
#include <awe_cluster_sequence.h>
#include <awe_cluster_configuration.h>
#include "fas_aggregation_context.h"
#include "fas_fetch_context.h"
#include "map_descriptor.h"
#include <vector>



enum class AWE_CLUSTER_MODE : int { CONV_3X3 = 3, CONV_5X5 = 5, CONV_1X1 = 1, POOL_3X3 = 13, POOL_5X5 = 15 };

class awe_cluster : public adapt_net_component
{
public:
	awe_cluster(int num_awe, adapt_net_context_id id);
	~awe_cluster();

	void set_kernels(std::vector<kernel*> kernels);
	void set_map(map_descriptor* map);

	void set_kernel_group(kernel_group* kg);
	void set_cluster_fas_fetch_context(fas_fetch_context* context);
	void set_cluster_fas_aggregate_context(fas_aggregation_context* context);
	void set_cluster_sequence(awe_cluster_sequence* seq);
	std::vector<awe_cluster_configuration*> get_configuration();

	bool has_cascade_target();
	bool has_cascade_source();
		
	void set_cascade_source(awe_cluster* awe);
	void set_cascade_target(awe_cluster* awe);

	void print();
protected:
	virtual void do_print_id();
	virtual void do_print_cascade_configuration();
	virtual void do_print_kernel_configuration();
	virtual void do_print_fas_fetch_context();
	virtual void do_print_fas_aggregate_context();
	virtual void do_print_map_ingress_configuration();
	virtual void do_print_map_egress_configuration();


private:
	

	awe_cluster* m_SourceAWE;
	awe_cluster* m_TargetAWE;

	std::vector<kernel*> m_Kernels;
	map_descriptor* m_Map;

	kernel_group* m_KernelGroup;
	awe_cluster_sequence* m_SequenceConfig;
	fas_fetch_context* m_FetchContext;
	fas_aggregation_context* m_AggregateContext;

	std::vector<awe_cluster_configuration*> get_weight_configuration(); 
	awe_cluster_configuration* get_sequence_configuration();
};

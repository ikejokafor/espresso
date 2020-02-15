#include "awe_cluster.h"
#include <awp_weight_configuration.h>
#include <awp_sequence_configuration.h>

#include <iostream>

awe_cluster::awe_cluster(int num_awe, adapt_net_context_id id) 
	: adapt_net_component(ADAPT_NET_COMPONENT_TYPE::AWE_CLUSTER, id)
{
	
	//for (int i = 0; i < num_awe; i++)
	//	m_AWE.push_back(new awe());

	m_AggregateContext = nullptr;
	m_SourceAWE = nullptr;
	m_TargetAWE = nullptr;
}

awe_cluster::~awe_cluster()
{
	//for (int i = 0; i < m_AWE.size(); i++)
	//	delete(m_AWE[i]);
}

void awe_cluster::set_kernels(std::vector<kernel*> kernels)
{
	m_Kernels = kernels;
}

void awe_cluster::set_map(map_descriptor * map)
{
	m_Map = map;
}

void awe_cluster::set_kernel_group(kernel_group * kg)
{
	m_KernelGroup = kg;
}

void awe_cluster::set_cluster_fas_fetch_context(fas_fetch_context * context)
{
	m_FetchContext = context;
}

void awe_cluster::set_cluster_fas_aggregate_context(fas_aggregation_context * context)
{
	m_AggregateContext = context;
}

void awe_cluster::set_cluster_sequence(awe_cluster_sequence * seq)
{
	m_SequenceConfig = seq;
}

std::vector<awe_cluster_configuration*> awe_cluster::get_configuration()
{
	std::vector<awe_cluster_configuration*> config_set;

	config_set = get_weight_configuration();
	config_set.push_back(get_sequence_configuration());

	return config_set;
}

bool awe_cluster::has_cascade_target()
{
	return (m_TargetAWE != nullptr);
}

bool awe_cluster::has_cascade_source()
{
	return (m_SourceAWE != nullptr);
}

void awe_cluster::set_cascade_source(awe_cluster * awe)
{
	m_SourceAWE = awe;
}

void awe_cluster::set_cascade_target(awe_cluster * awe)
{
	m_TargetAWE = awe;
}

void awe_cluster::print()
{
	do_print_id();
	do_print_cascade_configuration();
	do_print_kernel_configuration();
	do_print_map_ingress_configuration();
	do_print_map_egress_configuration();
}

void awe_cluster::do_print_id()
{
	std::cout << "\tConfiguration - \tAWE Cluster [" << get_id().unit_id << "," << get_id().context_id << "]" << std::endl;
}

void awe_cluster::do_print_cascade_configuration()
{
}

void awe_cluster::do_print_kernel_configuration()
{
	m_KernelGroup->print();
}

void awe_cluster::do_print_fas_fetch_context()
{
	m_FetchContext->print();
}

void awe_cluster::do_print_fas_aggregate_context()
{
	m_AggregateContext->print();
}

void awe_cluster::do_print_map_ingress_configuration()
{
}

void awe_cluster::do_print_map_egress_configuration()
{
}

std::vector<awe_cluster_configuration*> awe_cluster::get_weight_configuration()
{
	int weight_config_length;
	uint8_t* weight_config_bytes;
	kernel_group_config* weight_config_kgc;
	awe_cluster_configuration* weight_config;
	std::vector<awe_cluster_configuration*> weight_config_vector;

	//for (int i = 0; i < m_KernelGroups.size(); i++)
	{
		weight_config = new awe_cluster_configuration(/*new awp_weight_configuration()*/);
		weight_config_kgc = m_KernelGroup->get_config();
		weight_config_kgc->print();

		weight_config_bytes = weight_config_kgc->get_bytes(weight_config_length);
		weight_config->add_bytes(weight_config_bytes, weight_config_length);

		weight_config_vector.push_back(weight_config);
	}

	return weight_config_vector;
}

awe_cluster_configuration* awe_cluster::get_sequence_configuration()
{
	int sequence_config_length;
	uint8_t* sequence_config_bytes;

	awe_cluster_configuration* sequence_config = new awe_cluster_configuration(/*new awp_sequence_configuration()*/);

	sequence_config_bytes = m_SequenceConfig->get_bytes(sequence_config_length);
	sequence_config->add_bytes(sequence_config_bytes, sequence_config_length);

	return sequence_config;
}
#pragma once
#include "adapt_net_configuration_list.h"
#include "adapt_net_context_id.h"
#include "map_descriptor.h"

#include <vector>

enum class ADAPT_NET_COMPONENT_TYPE {FAS_STORE_CONTEXT, FAS_FETCH_CONTEXT, FAS_CONFIG_CONTEXT, FAS_AGGREGATION_CONTEXT, AWE, AWE_CLUSTER, AWE_CLUSTER_CASCADE, AWP};
class adapt_net_component
{
public :

	adapt_net_component(ADAPT_NET_COMPONENT_TYPE type, adapt_net_context_id id) {
		m_ID = id;
		m_Type = type;
		m_Address = adapt_net_header_address::from_context_id(m_ID);
	}

	ADAPT_NET_COMPONENT_TYPE get_type() { 
		return m_Type; 
	}

	adapt_net_context_id get_id() { 
		return m_ID; 
	}

	virtual void print() {
	};

	virtual void connect(adapt_net_component* target) { 
		m_Targets.push_back(target); 
		target->m_Sources.push_back(this); 
	}

	virtual std::vector<adapt_net_component*> get_sources() {
		return m_Sources;
	}

	virtual std::vector<adapt_net_component*> get_targets() { 
		return m_Targets; 
	}

	std::vector<adapt_net_configuration_list*> get_configuration() { 
		return do_get_configuration(); 
	}

	virtual map_descriptor* get_map_descriptor() {
		return nullptr;
	}

protected:
	adapt_net_component() {
	};

	virtual std::vector<adapt_net_configuration_list*> do_get_configuration() { 
		return std::vector<adapt_net_configuration_list*>(); 
	}
		
private :
	ADAPT_NET_COMPONENT_TYPE m_Type;
	adapt_net_context_id m_ID;

	std::vector<adapt_net_component*> m_Sources;
	std::vector<adapt_net_component*> m_Targets;
};
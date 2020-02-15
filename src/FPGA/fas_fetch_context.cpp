#include "fas_fetch_context.h"
#include "fas_fetch_context_configuration.h"

#include <iostream>

fas_fetch_context::fas_fetch_context(adapt_net_context_id id)
	: adapt_net_component(ADAPT_NET_COMPONENT_TYPE::FAS_FETCH_CONTEXT, id)
{
}

fas_fetch_context::~fas_fetch_context()
{
}

void fas_fetch_context::set_map_descriptor(map_descriptor * map)
{
	m_Map = map;
}

void fas_fetch_context::print()
{
	std::cout << "\tConfiguration - \tFAS Fetch Context [" << get_id().unit_id << "," << get_id().context_id << "]" << std::endl;
}

std::vector<adapt_net_configuration_list*> fas_fetch_context::do_get_configuration()
{
	//1. Get the destination address of the fas we are configuring
	//2. Get the destination address of the awe_cluster we are fetching for
	//3. Get the map dimension information 
	//4. Create configuration list immediate entry
	std::vector<adapt_net_configuration_list*> config_list;
	adapt_net_header_address tgt_address;

	auto tgt_awe_clusters = get_targets();

	//Should only be fetching for one AWE Cluster
	if (tgt_awe_clusters.size() == 1) {
		auto tgt_awe_cluster = tgt_awe_clusters.front();
		if (tgt_awe_cluster != nullptr) {
			tgt_address = tgt_awe_cluster->get_address();

			auto map = get_map_descriptor();
			auto map_bpp = map->get_bytes_per_pixel();
			auto map_length = map->get_length() * map_bpp;
			auto map_row_length = map->get_length(0) * map_bpp;
			auto map_address = map->get_address();

			auto fas_config = new fas_fetch_context_configuration(get_address());
			fas_config->set_image_length(map_length);
			fas_config->set_row_length(map_row_length);
			fas_config->set_awe_destination(tgt_address);
			fas_config->set_context_id(5);
			fas_config->set_address(map_address);

			config_list.push_back(fas_config);
		}
	}

	return config_list;
}

map_descriptor * fas_fetch_context::get_map_descriptor()
{
	return m_Map;
}

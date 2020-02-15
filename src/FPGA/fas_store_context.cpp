#include "fas_store_context.h"
#include "fas_store_context_configuration.h"

#include <iostream>

fas_store_context::fas_store_context(adapt_net_context_id id)
	: adapt_net_component(ADAPT_NET_COMPONENT_TYPE::FAS_STORE_CONTEXT,id)
{
}

fas_store_context::~fas_store_context()
{
}

void fas_store_context::set_map_descriptor(map_descriptor * map)
{
	m_Map = map;
}

void fas_store_context::print()
{
	std::cout << "\tConfiguration - \tFAS Store Context [" << get_id().unit_id << "," << get_id().context_id << "]" << std::endl;
	/*
	auto src = get_map_source();

	if (src.size() == 1) {
		std::cout << "Map Source Configuration" << std::endl;
		src.front()->print();
	}
	else {
		std::cout << "------------------------------------------" << std::endl;
	}
	*/
}

std::vector<adapt_net_configuration_list*> fas_store_context::do_get_configuration()
{
	//1. Get the destination address of the fas we are configuring
	//2. Get the destination address of the awe_cluster we are fetching for
	//3. Get the map dimension information 
	//4. Create configuration list immediate entry
	std::vector<adapt_net_configuration_list*> config_list;
	adapt_net_header_address tgt_address;

	auto map_sources = get_sources();

	//Should only be source for the store context

	auto map = get_map_descriptor();
	if (map != nullptr) {
		//auto map = new map_descriptor(5,5,5);
		auto map_bpp = map->get_bytes_per_pixel();
		auto map_length = map->get_length() * map_bpp;
		auto map_row_length = map->get_length(0) * map_bpp;
		auto map_address = map->get_address();

		auto fas_config = new fas_store_context_configuration(get_address());
		fas_config->set_image_length(map_length);
		fas_config->set_row_length(map_row_length);
		//fas_config->set_awe_destination(tgt_address);
		fas_config->set_context_id(5);
		fas_config->set_address(map_address);

		config_list.push_back(fas_config);
	}

	return config_list;
}

map_descriptor * fas_store_context::get_map_descriptor()
{
	map_descriptor* map = nullptr;

	auto map_sources = get_sources();

	//Should only be source for the store context
	if (map_sources.size() == 1) {
		auto map_source = map_sources.front();
		if (map_source != nullptr) {
			map = map_source->get_map_descriptor();
		}
	}
	
	return new map_descriptor(5, 5, 5);
	//return map;
}

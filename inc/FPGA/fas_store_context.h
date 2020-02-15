#pragma once
#include "adapt_net_component.h"
#include "map_descriptor.h"
#include <vector>

class fas_store_context : public adapt_net_component
{
public:
	fas_store_context(adapt_net_context_id id);
	~fas_store_context();

	void set_map_descriptor(map_descriptor* map);
	void print();
protected:
	std::vector<adapt_net_configuration_list*> do_get_configuration();
private:
	map_descriptor * m_Map;

	map_descriptor* get_map_descriptor();
};
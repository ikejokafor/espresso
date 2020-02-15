#pragma once

#include <stdint.h>
#include <vector>
#include "adapt_net_management_header.h"
#include "adapt_net_configuration_list_entry.h"


class adapt_net_configuration_list
{
public:
	adapt_net_configuration_list(CONFIGURATION_ENTRY_TYPE type);
	virtual ~adapt_net_configuration_list() = 0;

	CONFIGURATION_ENTRY_TYPE get_type();

	virtual adapt_net_management_header* get_header();
	virtual void set_header(adapt_net_management_header* header);

	virtual uint16_t get_length();
	virtual void set_length(uint16_t value);

	std::vector<adapt_net_configuration_list_entry*> get_entries();

protected:
	virtual std::vector<adapt_net_configuration_list_entry*> do_get_entries();
	CONFIGURATION_ENTRY_TYPE m_Type;
	uint16_t m_Length;
	adapt_net_management_header* m_ManagementConfigHeader;
};
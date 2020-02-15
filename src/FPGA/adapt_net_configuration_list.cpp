#include "adapt_net_configuration_list.h"

adapt_net_configuration_list::adapt_net_configuration_list(CONFIGURATION_ENTRY_TYPE type) : m_Type(type)
{
}

adapt_net_configuration_list::~adapt_net_configuration_list()
{
}

CONFIGURATION_ENTRY_TYPE adapt_net_configuration_list::get_type()
{
	return m_Type;
}

adapt_net_management_header * adapt_net_configuration_list::get_header()
{
	return m_ManagementConfigHeader;
}

void adapt_net_configuration_list::set_header(adapt_net_management_header * header)
{
	m_ManagementConfigHeader = header;
}

uint16_t adapt_net_configuration_list::get_length()
{
	return m_Length;
}

void adapt_net_configuration_list::set_length(uint16_t value)
{
	m_Length = value;
}

std::vector<adapt_net_configuration_list_entry*> adapt_net_configuration_list::get_entries()
{
	return do_get_entries();
}

std::vector<adapt_net_configuration_list_entry*> adapt_net_configuration_list::do_get_entries()
{
	return std::vector<adapt_net_configuration_list_entry*>();
}

#include "fas.h"

int fas::fas_id = 0;

fas::fas()
{
	m_ID = get_fas_id();
}

fas::fas(int num_fetch_context, int num_config_context, int num_store_context)
{
	m_ID = get_fas_id();
	m_NumConfigContext = num_config_context;
	m_NumFetchContext = num_fetch_context;
	m_NumStoreContext = num_store_context;
	m_NumAggregationContext = num_store_context;
}

fas::~fas()
{
	//for (std::size_t i = 0; i < m_FetchContext.size(); ++i)
	//	delete(m_FetchContext[i]);
}

void fas::initialize()
{
	m_ConfigContext.clear();
	m_FetchContext.clear();
	m_StoreContext.clear();
	m_AggregationContext.clear();

	for (int i = 0; i < m_NumConfigContext; i++) {
		m_ConfigContext.push_back(new fas_config_context(adapt_net_context_id(m_ID, i)));
	}

	for (int i = 0; i < m_NumFetchContext; i++) {
		m_FetchContext.push_back(new fas_fetch_context(adapt_net_context_id(m_ID, i)));
	}

	for (int i = 0; i<m_NumAggregationContext; i++) {
		m_AggregationContext.push_back(new fas_aggregation_context(adapt_net_context_id(m_ID, i)));
	}

	for (int i = 0; i < m_NumStoreContext; i++) {
		m_StoreContext.push_back(new fas_store_context(adapt_net_context_id(m_ID, i)));
	}
}

std::vector<fas_config_context*> fas::get_config_context()
{
	return m_ConfigContext;
}

std::vector<fas_fetch_context*> fas::get_fetch_context()
{
	return m_FetchContext;
}

std::vector<fas_aggregation_context*> fas::get_aggregation_context()
{
	return m_AggregationContext;
}

std::vector<fas_store_context*> fas::get_store_context()
{
	return m_StoreContext;
}

#pragma once

#include <vector>
#include "fas_aggregation_context.h"
#include "fas_config_context.h"
#include "fas_fetch_context.h"
#include "fas_store_context.h"

enum class FAS_FUNCTION { RESULT_STORE, RESULT_ACCUMULATE, DATA_FETCH };

class fas
{
public:
	fas();
	fas(int num_fetch_context, int num_config_context, int num_store_context);
	~fas();

	void initialize();

	std::vector<fas_config_context*> get_config_context();
	std::vector<fas_fetch_context*> get_fetch_context();
	std::vector<fas_aggregation_context*> get_aggregation_context();
	std::vector<fas_store_context*> get_store_context();

private:
	static int fas_id;
	static int get_fas_id() {
		return fas_id++;
	}

	int m_ID;
	std::vector<fas_config_context*> m_ConfigContext;
	std::vector<fas_fetch_context*> m_FetchContext;
	std::vector<fas_store_context*> m_StoreContext;
	std::vector<fas_aggregation_context*> m_AggregationContext;

	int m_NumConfigContext;
	int m_NumFetchContext; 
	int m_NumAggregationContext;
	int m_NumStoreContext;

	
};

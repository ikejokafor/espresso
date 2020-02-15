#include "awp.h"

int awp::conv_3x3_dim_min[3] = { 5,5,1 };
int awp::conv_5x5_dim_min[3] = { 7,7,1 };
int awp::conv_1x1_dim_min[3] = { 1,1,1 };
int awp::pool_3x3_dim_min[3] = { 3,3,1 };
int awp::pool_5x5_dim_min[3] = { 5,5,1 };

int awp::conv_3x3_dim_max[3] = { 512,512,512 };
int awp::conv_5x5_dim_max[3] = { 512,512,512 };
int awp::conv_1x1_dim_max[3] = { 512,512,512 };
int awp::pool_3x3_dim_max[3] = { 512,512,512 };
int awp::pool_5x5_dim_max[3] = { 512,512,512 };

int awp::awp_id = 0;

awp::awp()
{
	m_ID = get_awp_id();
}

awp::awp(int num_clusters, int num_awe)
{
	m_ID = get_awp_id();
	m_NumAwe = num_awe;
	m_NumClusters = num_clusters;
}

awp::~awp()
{
	for (std::size_t i = 0; i < m_Clusters.size(); ++i)
		delete(m_Clusters[i]);
}

awe_cluster * awp::get_awe_cluster(int index)
{
	if (index >= m_Clusters.size())
		return nullptr;

	return m_Clusters[index];
}

std::vector<awe_cluster*> awp::get_awe_clusters()
{
	return m_Clusters;
}

void awp::add_awe_cluster(awe_cluster * cluster)
{
	m_Clusters.push_back(cluster);
}

int awp::get_awe_cluster_simd(AWE_CLUSTER_MODE mode)
{
	switch (mode)
	{
	case AWE_CLUSTER_MODE::CONV_3X3 :
		return 8;
		break;
	case AWE_CLUSTER_MODE::CONV_5X5:
		return 4;
		break;
	case AWE_CLUSTER_MODE::CONV_1X1 :
		return 100;
		break;
	case AWE_CLUSTER_MODE::POOL_3X3:
		return 8;
		break;
	case AWE_CLUSTER_MODE::POOL_5X5:
		return 4;
		break;
	}
	return 0;
}



int awp::get_awe_cluster_map_dimension_minimum(int dim, AWE_CLUSTER_MODE mode)
{
	switch (mode)
	{
	case AWE_CLUSTER_MODE::CONV_3X3:
		return conv_3x3_dim_min[dim];
		break;
	case AWE_CLUSTER_MODE::CONV_5X5:
		return conv_5x5_dim_min[dim];
		break;
	case AWE_CLUSTER_MODE::CONV_1X1:
		return conv_1x1_dim_min[dim];
		break;
	case AWE_CLUSTER_MODE::POOL_3X3:
		return pool_3x3_dim_min[dim];
		break;
	case AWE_CLUSTER_MODE::POOL_5X5:
		return pool_5x5_dim_min[dim];
		break;
	}

	return 0;
}

int awp::get_awe_cluster_map_dimension_maximum(int dim, AWE_CLUSTER_MODE mode)
{
	switch (mode)
	{
	case AWE_CLUSTER_MODE::CONV_3X3:
		return conv_3x3_dim_max[dim];
		break;
	case AWE_CLUSTER_MODE::CONV_5X5:
		return conv_5x5_dim_max[dim];
		break;
	case AWE_CLUSTER_MODE::CONV_1X1:
		return conv_1x1_dim_max[dim];
		break;
	case AWE_CLUSTER_MODE::POOL_3X3:
		return pool_3x3_dim_max[dim];
		break;
	case AWE_CLUSTER_MODE::POOL_5X5:
		return pool_5x5_dim_max[dim];
		break;
	}

	return 0;
}

void awp::initialize()
{
	m_Clusters.clear();

	for (int i = 0; i < m_NumClusters; i++)
		add_awe_cluster(new awe_cluster(m_NumAwe, adapt_net_context_id(m_ID, i)));
}

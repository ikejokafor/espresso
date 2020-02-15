#pragma once
#include <awe_cluster.h>
#include <vector>

class awp
{
public :
	awp();
	awp(int num_clusters, int num_awe = 4);
	~awp();

	awe_cluster* get_awe_cluster(int index);
	std::vector<awe_cluster*> get_awe_clusters();

	void initialize();

	void add_awe_cluster(awe_cluster* cluster);
	
	static int get_awe_cluster_simd(AWE_CLUSTER_MODE mode);

	static int get_awe_cluster_map_dimension_minimum(int dim, AWE_CLUSTER_MODE mode);

	static int get_awe_cluster_map_dimension_maximum(int dim, AWE_CLUSTER_MODE mode);
private :
	
	static int conv_3x3_dim_min[3];
	static int conv_5x5_dim_min[3];
	static int conv_1x1_dim_min[3];
	static int pool_3x3_dim_min[3];
	static int pool_5x5_dim_min[3];

	static int conv_3x3_dim_max[3];
	static int conv_5x5_dim_max[3];
	static int conv_1x1_dim_max[3];
	static int pool_3x3_dim_max[3];
	static int pool_5x5_dim_max[3];

	std::vector<awe_cluster*> m_Clusters;

	

	static int get_awp_id() {
		return awp_id++;
	}
	static int awp_id;

	int m_NumClusters;
	int m_NumAwe;
	int m_ID;
	
};

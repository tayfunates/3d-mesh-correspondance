#ifndef PATCH_BASED_SUM_OF_CENTER_DISTANCES_H
#define PATCH_BASED_SUM_OF_CENTER_DISTANCES_H

#include "TAFeature.h"
#include <vector>

namespace TAFea
{
	//One of the patch based features extracted for a vertex
	//Patches are extracted around a patch using PatchBasedPerVertexFeatureExtraction
	//This feature hold the distances of all vertices in a patch extracted around a specific vertex to center of the same patch
	//Since there can be several patches around the vertex, the descriptor is held in a vector
	class PatchBasedSumOfCenterDistances : public PerVertexFeature
	{
	public:
		virtual ~PatchBasedSumOfCenterDistances();
		PatchBasedSumOfCenterDistances();
		PatchBasedSumOfCenterDistances(const int& vertexid);

	public:
		std::vector<double> m_vDescriptor;
	};
}

#endif
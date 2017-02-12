#ifndef PATCH_BASED_SHAPE_DISTRIBUTION_HISTOGRAM_H
#define PATCH_BASED_SHAPE_DISTRIBUTION_HISTOGRAM_H

#include "TAFeature.h"
#include <vector>

namespace TAFea
{
	//One of the patch based features extracted for a vertex
	//Patches are extracted around a patch using PatchBasedPerVertexFeatureExtraction
	//For a vertex, several patches are extracted. For each patch a histogram is calculated by PatchBasedShapeDistributionDescExtraction
	//For each histogram, the number of samples in each bin may be normalized making the feature object a probability distribution function
	//The type of the distribution function from which the samples of the histogram is selected is given in PatchBasedShapeDistributionDescExtraction
	//Since there can be several patches around the vertex, and there is a histogram (probability distribution), the descriptor is held in a vector of vectors
	class PatchBasedShapeDistributionHistogram : public PerVertexFeature
	{
	public:
		virtual ~PatchBasedShapeDistributionHistogram();
		PatchBasedShapeDistributionHistogram();
		PatchBasedShapeDistributionHistogram(const int& vertexid);

	public:
		std::vector<std::vector<double> > m_vDescriptor;

	};
}

#endif
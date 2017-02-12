#include "PatchBasedShapeDistributionHistogram.h"

namespace TAFea
{
	PatchBasedShapeDistributionHistogram::PatchBasedShapeDistributionHistogram()
	{
		this->m_ID = -1;
	}

	PatchBasedShapeDistributionHistogram::PatchBasedShapeDistributionHistogram(const int& vertexid)
	{
		this->m_ID = vertexid;
	}

	PatchBasedShapeDistributionHistogram::~PatchBasedShapeDistributionHistogram()
	{

	}
}
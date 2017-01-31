#include "PatchBasedSumOfCenterDistances.h"

namespace TAFea
{
	PatchBasedSumOfCenterDistances::PatchBasedSumOfCenterDistances()
	{
		this->m_ID = -1;
	}

	PatchBasedSumOfCenterDistances::PatchBasedSumOfCenterDistances(const int& vertexid)
	{
		this->m_ID = vertexid;
	}

	PatchBasedSumOfCenterDistances::~PatchBasedSumOfCenterDistances()
	{

	}
}
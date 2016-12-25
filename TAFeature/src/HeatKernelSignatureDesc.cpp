#include "HeatKernelSignatureDesc.h"

namespace TAFea
{

	HeatKernelSignatureDesc::HeatKernelSignatureDesc()
	{
		this->m_ID = -1;
	}

	HeatKernelSignatureDesc::HeatKernelSignatureDesc(const int& vertexid)
	{
		this->m_ID = vertexid;
	}

	HeatKernelSignatureDesc::~HeatKernelSignatureDesc()
	{

	}

	double HeatKernelSignatureDesc::L2Distance(const HeatKernelSignatureDesc& hks1, const HeatKernelSignatureDesc& hks2)
	{
		if (hks1.m_vDescriptor.size() != hks2.m_vDescriptor.size())
		{
			return -1.0;
		}

		double distance = 0.0;
		for (size_t i = 0; i < hks1.m_vDescriptor.size(); i++)
		{
			const double diff = hks1.m_vDescriptor[i] - hks2.m_vDescriptor[i];
			distance += diff * diff;
		}
		return sqrt(distance);
	}
}
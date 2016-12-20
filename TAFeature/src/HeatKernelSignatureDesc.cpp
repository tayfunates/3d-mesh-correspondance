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
}
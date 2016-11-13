#include "IntrinsicWaveDesc.h"

namespace TAFea
{

	IntrinsicWaveDesc::IntrinsicWaveDesc()
	{
		this->m_ID = -1;
	}

	IntrinsicWaveDesc::IntrinsicWaveDesc(const int& vertexid)
	{
		this->m_ID = vertexid;
	}

	IntrinsicWaveDesc::~IntrinsicWaveDesc()
	{

	}
}
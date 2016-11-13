#include "IntrinsicWaveDescExtraction.h"
#include <core/TriangularMesh.h>

namespace TAFeaExt
{
	IntrinsicWaveDescExtraction::~IntrinsicWaveDescExtraction()
	{

	}

	IntrinsicWaveDescExtraction::IntrinsicWaveDescExtraction()
	{

	}

	IntrinsicWaveDescExtraction::IntrinsicWaveDescExtraction(const IntrinsicWaveDescExtraction& other)
	{

	}

	Result IntrinsicWaveDescExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		return TACore::TACORE_OK;
	}

	Result IntrinsicWaveDescExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}



		return TACORE_OK;
	}
}
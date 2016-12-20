#include "HKSDescExtraction.h"
#include <core/TriangularMesh.h>

namespace TAFeaExt
{
	HKSDescExtraction::~HKSDescExtraction()
	{

	}

	HKSDescExtraction::HKSDescExtraction()
	{

	}

	HKSDescExtraction::HKSDescExtraction(const HKSDescExtraction& other)
	{

	}

	Result HKSDescExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		return TACore::TACORE_OK;
	}

	Result HKSDescExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		return TACORE_OK;
	}
}
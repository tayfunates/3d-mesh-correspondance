#include "OnEdgeAvgGeoDistExtraction.h"
#include "AvgGeodesicDistance.h"

namespace TAFeaExt
{
	OnEdgeAvgGeoDistExtraction::~OnEdgeAvgGeoDistExtraction()
	{
		
	}

	OnEdgeAvgGeoDistExtraction::OnEdgeAvgGeoDistExtraction()
	{

	}

	OnEdgeAvgGeoDistExtraction::OnEdgeAvgGeoDistExtraction(const OnEdgeAvgGeoDistExtraction& other)
	{

	}

	Result OnEdgeAvgGeoDistExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeature*>& outFeatures)
	{
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		return TACore::TACORE_OK;
	}

	Result OnEdgeAvgGeoDistExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeature **outFeature)
	{
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		LocalFeature *resDist = new AvgGeodesicDistance(id, 5.0f);
		*outFeature = resDist;
		return TACore::TACORE_OK;
	}
}
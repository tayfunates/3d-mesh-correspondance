#include "OnEdgeAvgGeoDistExtraction.h"
#include "AvgGeodesicDistance.h"
#include "GeodesicDistanceMatrixExtraction.h"
#include <core/TriangularMesh.h>
#include <numeric>

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

	Result OnEdgeAvgGeoDistExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}
		
		Result result = TACORE_OK;

		GeodesicDistanceMatrixExtraction geoDistMatrixExtraction;
		geoDistMatrixExtraction.setGeodesicDistanceType(GeodesicDistanceMatrix::ON_EDGE_GEODESIC);

		GlobalFeaturePtr globalFeaPtr;
		result = geoDistMatrixExtraction.extract(mesh, globalFeaPtr);

		if (result == TACORE_OK)
		{
			GeodesicDistanceMatrix* geoMatrixDescPtr = (GeodesicDistanceMatrix*)(globalFeaPtr.get());
			TriangularMesh *triMesh = (TriangularMesh*)mesh;
			const size_t verSize = triMesh->verts.size();
			outFeatures = std::vector<LocalFeaturePtr>(verSize);

			for (size_t v = 0; v < verSize; v++)
			{
				float avgVal = 0.0f;
				for (int w = 0; w < geoMatrixDescPtr->m_GeoMatrix.cols(); w++)
				{
					avgVal += geoMatrixDescPtr->m_GeoMatrix.getVal(v, w);
				}
				avgVal /= geoMatrixDescPtr->m_GeoMatrix.cols();

				LocalFeature *avgGeoDistDescPtr = new AvgGeodesicDistance(v, avgVal);
				outFeatures[v] = LocalFeaturePtr(avgGeoDistDescPtr);
			}
		}

		return result;
	}

	Result OnEdgeAvgGeoDistExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		Result result = TACORE_OK;

		GeodesicDistanceMatrixExtraction geoDistMatrixExtraction;
		geoDistMatrixExtraction.setGeodesicDistanceType(GeodesicDistanceMatrix::ON_EDGE_GEODESIC);

		std::vector<float> geoDistancesFromVertex;
		result = geoDistMatrixExtraction.extract(triMesh, id, geoDistancesFromVertex);

		if (result == TACORE_OK)
		{
			float avgDistance = std::accumulate(geoDistancesFromVertex.begin(), geoDistancesFromVertex.end(), 0.0f) / ((float)triMesh->verts.size());
			LocalFeature *avgGeoDistDescPtr = new AvgGeodesicDistance(id, avgDistance);

			outFeaturePtr = LocalFeaturePtr(avgGeoDistDescPtr);
		}

		return result;
	}
}

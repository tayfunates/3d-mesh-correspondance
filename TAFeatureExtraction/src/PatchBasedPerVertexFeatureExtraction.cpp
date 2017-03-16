#include "PatchBasedPerVertexFeatureExtraction.h"
#include <core/TriangularMesh.h>
#include "GeodesicDistanceMatrixExtraction.h"

namespace TAFeaExt
{
	PatchBasedPerVertexFeatureExtraction::~PatchBasedPerVertexFeatureExtraction()
	{

	}

	PatchBasedPerVertexFeatureExtraction::PatchBasedPerVertexFeatureExtraction()
	{
		m_fMinGeodesicRadius = 0.0f;
		m_fMaxGeodesicRadius = 0.0f;
		m_nNumberOfPatches = 0;
		m_pGeodeticDistanceMatrix = NULL;
	}

	PatchBasedPerVertexFeatureExtraction::PatchBasedPerVertexFeatureExtraction(const PatchBasedPerVertexFeatureExtraction& other)
	{
		m_fMinGeodesicRadius = other.m_fMinGeodesicRadius;
		m_fMaxGeodesicRadius = other.m_fMaxGeodesicRadius;
		m_nNumberOfPatches = other.m_nNumberOfPatches;
		m_pGeodeticDistanceMatrix = other.m_pGeodeticDistanceMatrix;
	}

	Result PatchBasedPerVertexFeatureExtraction::extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		Result result = TACore::TACORE_OK;

		GeodesicDistanceMatrixExtraction geoDistMatrixExtraction;
		geoDistMatrixExtraction.setGeodesicDistanceType(GeodesicDistanceMatrix::ON_EDGE_GEODESIC);

		//If geodesic distance matrix is not set, calculate it
		GlobalFeaturePtr globalFeaPtr; //Let it live if calculate the matrix
		if (this->m_pGeodeticDistanceMatrix == NULL)
		{	
			result = geoDistMatrixExtraction.extract(mesh, globalFeaPtr);
			m_pGeodeticDistanceMatrix = (GeodesicDistanceMatrix*)(globalFeaPtr.get());
		}

		std::vector<PatchList> patchesForAll;
		result = createPatches(triMesh, m_pGeodeticDistanceMatrix->m_GeoMatrix, this->m_fMinGeodesicRadius, this->m_fMaxGeodesicRadius, this->m_nNumberOfPatches, patchesForAll);

		if (result == TACore::TACORE_OK)
		{
			result = calcFeature(triMesh, patchesForAll, outFeatures);
		}

		return result;
	}

	Result PatchBasedPerVertexFeatureExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		Result result = TACore::TACORE_OK;

		GeodesicDistanceMatrixExtraction geoDistMatrixExtraction;
		geoDistMatrixExtraction.setGeodesicDistanceType(GeodesicDistanceMatrix::ON_EDGE_GEODESIC);

		std::vector<float> distances;
		result = geoDistMatrixExtraction.extract(triMesh, id, distances);

		PatchList patches;
		result = createVertexPatches(triMesh, id, distances, this->m_fMinGeodesicRadius, this->m_fMaxGeodesicRadius, this->m_nNumberOfPatches, patches);

		if (result == TACore::TACORE_OK)
		{
			result = calcFeature(triMesh, id, patches, outFeaturePtr);
		}

		return result;
	}

	Result PatchBasedPerVertexFeatureExtraction::createPatches(TriangularMesh* triMesh, const TAMatrix<float>& distanceMatrix, const float& minRadius, const float& maxRadius, const int& noOfPatches, std::vector<PatchList>& listOfPatchLists)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);
		listOfPatchLists.clear();

		const int noVertices = (int) triMesh->verts.size();
		listOfPatchLists = std::vector<PatchList>(noVertices);

		for (int v = 0; v < noVertices; v++)
		{
			if (v < noVertices - 1)
			{
				std::cout << "%" << (100 * v) / noVertices << " completed for creating all patches for all vertices" << "\r";
			}
			else
			{
				std::cout << "%" << 100 << " completed for creating all patches for all vertices" << "\n";
			}
			TACORE_CHECK_RESULT( createVertexPatches(triMesh, v, distanceMatrix, minRadius, maxRadius, noOfPatches, listOfPatchLists[v]) );
		}
		return TACore::TACORE_OK;
	}

	Result PatchBasedPerVertexFeatureExtraction::createVertexPatches(TriangularMesh* triMesh, const int& vertexId, const TAMatrix<float>& distanceMatrix, const float& minRadius, const float& maxRadius, const int& noOfPatches, PatchList& patchesVertexIds)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);
		patchesVertexIds.clear();

		const float rIncrement = (maxRadius - minRadius) / (noOfPatches - 1);
		float currentRadius = minRadius;

		patchesVertexIds = std::vector<std::vector<int> >(noOfPatches);
		int scale = 0;
		while (scale < noOfPatches)
		{
			for (int w = 0; w < distanceMatrix.cols(); w++)
			{
				if (distanceMatrix.getVal(vertexId, w) <= currentRadius)
				{
					patchesVertexIds[scale].push_back(w);
				}
			}

			currentRadius += rIncrement;
			scale++;
		}
		return TACore::TACORE_OK;
	}

	Result PatchBasedPerVertexFeatureExtraction::createVertexPatches(TriangularMesh* triMesh, const int& vertexId, const std::vector<float>& distanceVector, const float& minRadius, const float& maxRadius, const int& noOfPatches, PatchList& patchesVertexIds)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);
		patchesVertexIds.clear();

		const float rIncrement = (maxRadius - minRadius) / (noOfPatches - 1);
		float currentRadius = minRadius;

		patchesVertexIds = std::vector<std::vector<int> >(noOfPatches);
		int scale = 0;
		while (scale < noOfPatches)
		{
			for (int w = 0; w < (int) distanceVector.size(); w++)
			{
				if (distanceVector[w] <= currentRadius)
				{
					patchesVertexIds[scale].push_back(w);
				}
			}

			currentRadius += rIncrement;
			scale++;
		}
		return TACore::TACORE_OK;
	}

	void PatchBasedPerVertexFeatureExtraction::setMinGeodesicDistance(const float& minGeoDistance)
	{
		this->m_fMinGeodesicRadius = minGeoDistance;
	}

	void PatchBasedPerVertexFeatureExtraction::setMaxGeodesicDistance(const float& maxGeoDistance)
	{
		this->m_fMaxGeodesicRadius = maxGeoDistance;
	}

	float PatchBasedPerVertexFeatureExtraction::getMinGeodesicDistance() const
	{
		return this->m_fMinGeodesicRadius;
	}

	float PatchBasedPerVertexFeatureExtraction::getMaxGeodesicDistance() const
	{
		return this->m_fMaxGeodesicRadius;
	}

	void PatchBasedPerVertexFeatureExtraction::setNumberOfPatches(const int& noOfPatches)
	{
		this->m_nNumberOfPatches = noOfPatches;
	}

	int PatchBasedPerVertexFeatureExtraction::getNumberOfPatches() const
	{
		return this->m_nNumberOfPatches;
	}

	void PatchBasedPerVertexFeatureExtraction::setGeodesicDistanceMatrix(TAFea::GeodesicDistanceMatrix* gdMatrix)
	{
		this->m_pGeodeticDistanceMatrix = gdMatrix;
	}
}
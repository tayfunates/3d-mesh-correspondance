#include "GeodesicDistanceMatrixExtraction.h"
#include <core/TriangularMesh.h>
#include <core/FibHeap.h>

namespace TAFeaExt
{
	GeodesicDistanceMatrixExtraction::~GeodesicDistanceMatrixExtraction()
	{

	}

	GeodesicDistanceMatrixExtraction::GeodesicDistanceMatrixExtraction()
	{
		this->m_MatrixDistanceType = GeodesicDistanceMatrix::ON_EDGE_GEODESIC;
	}

	GeodesicDistanceMatrixExtraction::GeodesicDistanceMatrixExtraction(const GeodesicDistanceMatrixExtraction& other)
	{
		this->m_MatrixDistanceType = other.m_MatrixDistanceType;
	}

	Result GeodesicDistanceMatrixExtraction::extract(PolygonMesh *mesh, GlobalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		Result(GeodesicDistanceMatrixExtraction::*geo_dist_func_ptr)(TriangularMesh*, const int&, std::vector<float>&);
		if (this->m_MatrixDistanceType == GeodesicDistanceMatrix::ON_EDGE_GEODESIC)
		{
			geo_dist_func_ptr = &GeodesicDistanceMatrixExtraction::extractOnEdgeGeodesics;
		}
		else
		{
			geo_dist_func_ptr = NULL;
		}

		Result result = TACore::TACORE_OK;
		GlobalFeature *globalFeaPtr = NULL;
		//If the geodesic is not implemented or triMesh is null
		if (geo_dist_func_ptr == NULL || triMesh == NULL)
		{
			result = TACore::TACORE_INVALID_OPERATION;
		}
		else
		{
			GeodesicDistanceMatrix* geoDistanceMatrix = new GeodesicDistanceMatrix(this->m_MatrixDistanceType);
			const int N = (int)triMesh->verts.size();
			geoDistanceMatrix->m_GeoMatrix.init(N, N);

			for (int v = 0; v < N; v++)
			{
				if (v < N - 1)
				{
					std::cout << "%" << (100 * v) / N << " completed for calculating geodesic distance matrix" << "\r";
				}
				else
				{
					std::cout << "%" << 100 << " completed for calculating geodesic distance matrix" << "\n";
				}

				std::vector<float> distanceVec;
				result = (this->*geo_dist_func_ptr)(triMesh, v, distanceVec);

				geoDistanceMatrix->m_GeoMatrix.setRow(distanceVec.data(), v);
			}

			globalFeaPtr = geoDistanceMatrix;
		}
	
		outFeaturePtr = GlobalFeaturePtr(globalFeaPtr);
		return result;
	}

	Result GeodesicDistanceMatrixExtraction::extract(TriangularMesh *triMesh, const int& vid, std::vector<float>& distances)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);

		Result result = TACore::TACORE_OK;
		if (this->m_MatrixDistanceType == GeodesicDistanceMatrix::ON_EDGE_GEODESIC)
		{
			result = extractOnEdgeGeodesics(triMesh, vid, distances);
		}
		else
		{
			result = TACore::TACORE_INVALID_OPERATION;
		}
		return result;
	}

	Result GeodesicDistanceMatrixExtraction::extractOnEdgeGeodesics(TriangularMesh *triMesh, const int& vid, std::vector<float>& distances)
	{
		//Create the distance array which will be used during djsktra shortest path computation
		const int vertexCount = static_cast<int>(triMesh->verts.size());
		distances = std::vector<float>(vertexCount);

		//Create a heap node array. This array will hold the heap nodes created.
		//During decrease key, this array will be used to reach the nodes easily.
		FibHeapNode** heapNodeArray = new FibHeapNode*[vertexCount];

		//Create the fibonacci heap for fast computation
		FibHeap* fibHeap = new FibHeap();

		//Initialization
		for (int v = 0; v < vertexCount; v++)
		{
			//if the vertex is the current vertex, the current distance is 0, INFINITY otherwise
			distances[v] = (v == vid) ? 0.0f : INFINITY;
			heapNodeArray[v] = new FibHeapNode(distances[v], v);
			fibHeap->Insert(heapNodeArray[v]);
		}

		//Main loop of djsktra
		while (1)
		{
			//extract the vertex with the min current distance value
			FibHeapNode* minNode = fibHeap->ExtractMin(); //fast O(lgV) priority queue algo
			if (minNode == NULL)
			{
				//No vertex remained
				break;
			}
			int minDidx = minNode->element;
			float minD = minNode->key;

			//We do not need the current minNode anymore.
			TACORE_SAFE_DELETE(minNode);

			//relax each edge, including shortcuts, incident to minDidx
			const int vertexEdgeCount = static_cast<int>(triMesh->verts[minDidx]->edgeList.size());
			for (int ve = 0; ve < vertexEdgeCount; ve++)
			{
				int e = triMesh->verts[minDidx]->edgeList[ve];
				int va = (triMesh->edges[e]->v1i == minDidx ? triMesh->edges[e]->v2i : triMesh->edges[e]->v1i);
				if (distances[minDidx] + triMesh->edges[e]->length < distances[va])
				{
					distances[va] = distances[minDidx] + triMesh->edges[e]->length; //relaxation
					fibHeap->DecreaseKey(heapNodeArray[va], distances[va]); //worst O(lgV), amortized O(1)
				}
			}
		}

		//Deallocate the space needed for computation	
		TACORE_SAFE_DELETE(fibHeap);
		TACORE_SAFE_DELETE_ARRAY(heapNodeArray);

		return TACore::TACORE_OK;
	}

	void GeodesicDistanceMatrixExtraction::setGeodesicDistanceType(const GeodesicDistanceMatrix::DistanceType& type)
	{
		this->m_MatrixDistanceType = type;
	}

	GeodesicDistanceMatrix::DistanceType GeodesicDistanceMatrixExtraction::getGeodesicDistanceType() const
	{
		return this->m_MatrixDistanceType;
	}
}
#include "OnEdgeAvgGeoDistExtraction.h"
#include "AvgGeodesicDistance.h"
#include <core/TriangularMesh.h>
#include <core/FibHeap.h>

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
		//Test the input mesh type.
		//Currently, it has to be a triangular mesh
		return TACore::TACORE_OK;
	}

	Result OnEdgeAvgGeoDistExtraction::extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr)
	{
		if (!mesh || mesh->getPolygonType() != TA_TRIANGULAR)
		{
			return TACORE_BAD_ARGS;
		}

		//We, now, know that the mesh is a triangular mesh
		TriangularMesh *triMesh = (TriangularMesh*)mesh;

		//Create the distance array which will be used during djsktra shortest path computation
		const int vertexCount = static_cast<int>(triMesh->verts.size());
		float *distanceArray = new float[vertexCount];

		//Create a heap node array. This array will hold the heap nodes created.
		//During decrease key, this array will be used to reach the nodes easily.
		FibHeapNode** heapNodeArray = new FibHeapNode*[vertexCount];

		//Create the fibonacci heap for fast computation
		FibHeap* fibHeap = new FibHeap();

		//Initialization
		for (int v = 0; v < vertexCount; v++)
		{
			//if the vertex is the current vertex, the current distance is 0, INFINITY otherwise
			distanceArray[v] = (v == id) ? 0.0f : INFINITY;
			heapNodeArray[v] = new FibHeapNode(distanceArray[v], v);
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
				if (distanceArray[minDidx] + triMesh->edges[e]->length < distanceArray[va])
				{
					distanceArray[va] = distanceArray[minDidx] + triMesh->edges[e]->length; //relaxation
					fibHeap->DecreaseKey(heapNodeArray[va], distanceArray[va]); //worst O(lgV), amortized O(1)
				}
			}
		}

		//Output the average
		float outAvgGeodesicDist = 0.0f;
		for (int v = 0; v < vertexCount; v++)
		{
			outAvgGeodesicDist += distanceArray[v];
		}
		LocalFeature *resDist = new AvgGeodesicDistance(id, outAvgGeodesicDist / vertexCount);
		outFeaturePtr = LocalFeaturePtr(resDist);

		//Deallocate the space needed for computation	
		TACORE_SAFE_DELETE(fibHeap);
		TACORE_SAFE_DELETE_ARRAY(heapNodeArray);
		TACORE_SAFE_DELETE_ARRAY(distanceArray);

		return TACORE_OK;
	}
}

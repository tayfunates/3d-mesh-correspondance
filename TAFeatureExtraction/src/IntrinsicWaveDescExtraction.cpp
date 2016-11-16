#include "IntrinsicWaveDescExtraction.h"
#include <core/TriangularMesh.h>
#include <core/FibHeap.h>

namespace TAFeaExt
{
	IntrinsicWaveDescExtraction::~IntrinsicWaveDescExtraction()
	{

	}

	IntrinsicWaveDescExtraction::IntrinsicWaveDescExtraction()
	{
		this->m_fMaxGeodesicRadius = 0.0f;
		this->m_nNumberOfWavesSampled = 0;
	}

	IntrinsicWaveDescExtraction::IntrinsicWaveDescExtraction(const IntrinsicWaveDescExtraction& other)
	{
		this->m_fMaxGeodesicRadius = other.m_fMaxGeodesicRadius;
		this->m_nNumberOfWavesSampled = other.m_nNumberOfWavesSampled;
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

		//Marking of the triangles
		std::vector<bool> triangleMarkings(triMesh->tris.size(), false);

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

			if (minD > this->m_fMaxGeodesicRadius)
			{
				//If the current minimum is far away from the current radius
				//Break since no other node will be inside the wave
				break;
			}

			//Mark the triangles around the current vertex
			const int vertexTriangleCount = static_cast<int>(triMesh->verts[minDidx]->triList.size());
			for (int vt = 0; vt < vertexTriangleCount; vt++)
			{
				const int t = triMesh->verts[minDidx]->triList[vt];  //Global id of the triangle
				//We won't add the triangle if there is a vertex in it which has not encountered yet
				//What if that triangle is inside the wave even though it has an INFINITY distant vertex?
				//Then, add the triangle when you encounter that vertex, if the vertex is really outside the wave, we would not be here
				//Therefore, the triangle will be added if all vertices of it are encountered
				const int tv1 = triMesh->tris[t]->v1i;
				const int tv2 = triMesh->tris[t]->v2i;
				const int tv3 = triMesh->tris[t]->v3i;

				if (distanceArray[tv1] < INFINITY && distanceArray[tv2] < INFINITY && distanceArray[tv3] < INFINITY)
				{
					//If they are smaller than infinity, then they are also smaller then current radius since
					//we must encounter those vertices before the current min vertex
					triangleMarkings[t] = true;
				}
			}

			//Continue Djsktra
			//relax each edge, including shortcuts, incident to minDidx
			const int vertexEdgeCount = static_cast<int>(triMesh->verts[minDidx]->edgeList.size());
			for (int ve = 0; ve < vertexEdgeCount; ve++)
			{
				int e = triMesh->verts[minDidx]->edgeList[ve]; //Global id of the edge
				int va = (triMesh->edges[e]->v1i == minDidx ? triMesh->edges[e]->v2i : triMesh->edges[e]->v1i);
				if (distanceArray[minDidx] + triMesh->edges[e]->length < distanceArray[va])
				{
					distanceArray[va] = distanceArray[minDidx] + triMesh->edges[e]->length; //relaxation
					fibHeap->DecreaseKey(heapNodeArray[va], distanceArray[va]); //worst O(lgV), amortized O(1)
				}
			}
		}

		std::vector<int> edgesTriInteractionCounts(triMesh->edges.size(), 0);
		for (size_t t = 0; t < triangleMarkings.size(); t++)
		{
			if (triangleMarkings[t] == true)
			{
				//Increment the count of edges interacting with this triangle
				edgesTriInteractionCounts[triMesh->tris[t]->e1] += 1;
				edgesTriInteractionCounts[triMesh->tris[t]->e2] += 1;
				edgesTriInteractionCounts[triMesh->tris[t]->e3] += 1;
			}
		}

		//Compute the approximation of the perimeter of the wave
		float perimeter = 0.0f;
		for (size_t e = 0; e < edgesTriInteractionCounts.size(); e++)
		{
			if (edgesTriInteractionCounts[e] == 1)
			{
				perimeter += triMesh->edges[e]->length;
			}
		}

		std::cout << "Perimeter: " << perimeter << std::endl;

		////Output the average
		//float outAvgGeodesicDist = 0.0f;
		//for (int v = 0; v < vertexCount; v++)
		//{
		//	outAvgGeodesicDist += distanceArray[v];
		//}
		//LocalFeature *resDist = new AvgGeodesicDistance(id, outAvgGeodesicDist / vertexCount);
		//outFeaturePtr = LocalFeaturePtr(resDist);

		//Deallocate the space needed for computation	
		TACORE_SAFE_DELETE(fibHeap);
		TACORE_SAFE_DELETE_ARRAY(heapNodeArray);
		TACORE_SAFE_DELETE_ARRAY(distanceArray);

		return TACORE_OK;
	}

	void IntrinsicWaveDescExtraction::setMaxGeodesicRadius(const float& maxGeodesicRadius)
	{
		this->m_fMaxGeodesicRadius = maxGeodesicRadius;
	}

	void IntrinsicWaveDescExtraction::setNumberOfWavesSampled(const unsigned int& noOfWavesSampled)
	{
		this->m_nNumberOfWavesSampled = noOfWavesSampled;
	}

	float IntrinsicWaveDescExtraction::getMaxGeodesicRadius() const
	{
		return this->m_fMaxGeodesicRadius;
	}

	unsigned int IntrinsicWaveDescExtraction::setNumberOfWavesSampled() const
	{
		return this->m_nNumberOfWavesSampled;
	}
}
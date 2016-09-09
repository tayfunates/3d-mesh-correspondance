#include <core/TriangularMesh.h>

namespace TAShape
{
	TriangularMesh::TriangularMesh()
	{

	}

	TriangularMesh::~TriangularMesh()
	{
		clear();
	}

	TACore::Result TriangularMesh::clear()
	{
		for (size_t i = 0; i < verts.size(); i++)
		{
			TACORE_SAFE_DELETE(verts[i]);
		}
		for (size_t i = 0; i < tris.size(); i++)
		{
			TACORE_SAFE_DELETE(tris[i]);
		}
		for (size_t i = 0; i < edges.size(); i++)
		{
			TACORE_SAFE_DELETE(edges[i]);
		}
		verts.clear();
		tris.clear();
		edges.clear();
		return TACore::TACORE_OK;
	}

	TACore::Result TriangularMesh::loadObj(const char* meshFile)
	{
		FILE* fPtr;
		if (!(fPtr = fopen(meshFile, "r")))
		{
			return TACore::TACORE_FILE_ERROR;
		}

		//Clear old mesh
		clear();

		char type;	//type of line: vertex line (v) or face line (f)
		float a, b, c;	//for face lines casting these to int will suffice
		/*minEdgeLen = INF;
		maxEdgeLen = -INF;
		edgeLenTotal = 0.0f;*/
		while (fscanf(fPtr, "%c %f %f %f\n", &type, &a, &b, &c) != EOF) //go till the end of file
			if (type == 'v')
			{
			//assume none of verts are duplicated initially
			//a *= SCALE_FACTOR;
			//b *= SCALE_FACTOR;
			//c *= SCALE_FACTOR;
			float* coords = new float[3];
			coords[0] = a;
			coords[1] = b;
			coords[2] = c;
			addVertex(coords); //ND: no duplicate check
			}
			else //if (type == 'f')
			{
				addTriangle((int)a - 1, (int)b - 1, (int)c - 1); //-1: obj indices start at 1
				verts[(int)a - 1]->interior = verts[(int)b - 1]->interior = verts[(int)c - 1]->interior = false;
			}
	
		for (int v = 0; v < (int)verts.size(); v++)
		{
			if (verts[v]->triList.empty())
			{
				std::cout << "2D Dirichlet/ARAP regularization behaves weirdly w/ isolated vertices: get rid of v" << v << std::endl;
				fclose(fPtr);
				return TACore::TACORE_ERROR;
			}
		}
		fclose(fPtr);
		std::cout << "Mesh has " << (int)tris.size() << " tris, " << (int)verts.size() << " verts, " << (int)edges.size() << " edges\nInitialization done\n";
		return TACore::TACORE_OK;
	}

	TACore::Result TriangularMesh::loadOff(const char* fName)
	{
		FILE* fPtr;
		if (!(fPtr = fopen(fName, "r")))
		{
			return TACore::TACORE_FILE_ERROR;
		}

		//Clear old mesh
		clear();

		char off[25];
		fscanf(fPtr, "%s\n", &off); //cout << off << " type file\n";
		float a, b, c, d;	//for face lines and the 2nd line (that gives # of verts, faces, and edges) casting these to int will suffice
		fscanf(fPtr, "%f %f %f\n", &a, &b, &c);
		int nVerts = (int)a, v = 0;
		/*	minEdgeLen = INF;
		maxEdgeLen = -INF;
		edgeLenTotal = 0.0f;*/
		while (v++ < nVerts) //go till the end of verts coord section
		{
			fscanf(fPtr, "%f %f %f\n", &a, &b, &c);


			float* coords = new float[3];
			coords[0] = a;
			coords[1] = b;
			coords[2] = c;

			addVertex(coords);
		}
		//verts ready, time to fill triangles
		while (fscanf(fPtr, "%f %f %f %f\n", &d, &a, &b, &c) != EOF) //go till the end of file
		{
			addTriangle((int)a, (int)b, (int)c); //no -1 'cos idxs start from 0 for off files
		}

		for (int v = 0; v < (int)verts.size(); v++)
		{
			if (verts[v]->triList.empty())
			{
				std::cout << "2D Dirichlet/ARAP regularization behaves weirdly w/ isolated vertices: get rid of v" << v << std::endl;
				fclose(fPtr);
				return TACore::TACORE_ERROR;
			}
		}
		fclose(fPtr);

		std::cout << "Mesh has " << (int)tris.size() << " tris, " << (int)verts.size() << " verts, " << (int)edges.size() << " edges\nInitialization done\n";
		return TACore::TACORE_OK;
	}


	TACore::Result TriangularMesh::createCube(float sideLength)
	{
		float** coords = new float*[8], flbc[3] = { 10, 7, 5 }, delta[3] = { 0, 0, 0 };
		for (int v = 0; v < 8; v++)
		{
			coords[v] = new float[3];

			if (v == 1)
				delta[0] += sideLength;
			else if (v == 2)
				delta[1] += sideLength;
			else if (v == 3)
				delta[0] -= sideLength;
			else if (v == 4)
				delta[2] -= sideLength;
			else if (v == 5)
				delta[0] += sideLength;
			else if (v == 6)
				delta[1] -= sideLength;
			else if (v == 7)
				delta[0] -= sideLength;

			for (int c = 0; c < 3; c++)
				coords[v][c] = flbc[c] + delta[c];

			addVertex(coords[v]);
		}

		//connectivity
		addTriangle(0, 1, 2);
		addTriangle(0, 2, 3);

		addTriangle(1, 6, 2);
		addTriangle(6, 5, 2);

		addTriangle(7, 5, 6);
		addTriangle(7, 4, 5);

		addTriangle(0, 3, 7);
		addTriangle(3, 4, 7);

		addTriangle(0, 6, 1);
		addTriangle(0, 7, 6);

		addTriangle(3, 2, 5);
		addTriangle(3, 5, 4);

		return TACore::TACORE_OK;
	}

	TACore::Result TriangularMesh::addVertex(float* coords)
	{
		int idx = verts.size();
		verts.push_back(new Vertex(idx, coords));
		return TACore::TACORE_OK;
	}

	TACore::Result TriangularMesh::addTriangle(int v1i, int v2i, int v3i)
	{
		int idx = tris.size();
		tris.push_back(new Triangle(idx, v1i, v2i, v3i));

		verts[v1i]->triList.push_back(idx);
		verts[v2i]->triList.push_back(idx);
		verts[v3i]->triList.push_back(idx);

		if (!makeVertsNeighbors(v1i, v2i))
			addEdge(v1i, v2i);

		if (!makeVertsNeighbors(v1i, v3i))
			addEdge(v1i, v3i);

		if (!makeVertsNeighbors(v2i, v3i))
			addEdge(v2i, v3i);

		return TACore::TACORE_OK;
	}

	bool TriangularMesh::makeVertsNeighbors(int v, int w)
	{
		//try to make v and w neighbor; return true if they already are

		for (int check = 0; check < (int)verts[v]->vertList.size(); check++)
			if (verts[v]->vertList[check] == w)
				return true;

		verts[v]->vertList.push_back(w);
		verts[w]->vertList.push_back(v);
		return false;
	}

	inline float distanceBetween(float* a, float* b)
	{
		return (float)sqrt((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]) + (a[2] - b[2])*(a[2] - b[2]));
	}

	TACore::Result TriangularMesh::addEdge(int a, int b)
	{
		int idx = edges.size();
		edges.push_back(new Edge(idx, a, b));

		verts[a]->edgeList.push_back(idx);
		verts[b]->edgeList.push_back(idx);

		return TACore::TACORE_OK;
	}

	TACore::Result TriangularMesh::assignNormalsToTriangles()
	{
		for (int i = 0; i < this->tris.size(); i++)
		{
			TAVector pva; pva.x = this->verts[this->tris[i]->v1i]->coords[0]; pva.y = this->verts[this->tris[i]->v1i]->coords[1]; pva.z = this->verts[this->tris[i]->v1i]->coords[2];
			TAVector pvb; pvb.x = this->verts[this->tris[i]->v2i]->coords[0]; pvb.y = this->verts[this->tris[i]->v2i]->coords[1]; pvb.z = this->verts[this->tris[i]->v2i]->coords[2];
			TAVector pvc; pvc.x = this->verts[this->tris[i]->v3i]->coords[0]; pvc.y = this->verts[this->tris[i]->v3i]->coords[1]; pvc.z = this->verts[this->tris[i]->v3i]->coords[2];

			TAVector normal;
			TAVector lhs;
			TAVector rhs;
			lhs.x = pvb.x - pva.x;
			lhs.y = pvb.y - pva.y;
			lhs.z = pvb.z - pva.z;

			rhs.x = pvc.x - pva.x;
			rhs.y = pvc.y - pva.y;
			rhs.z = pvc.z - pva.z;

			normal = lhs * rhs;
			normal.normalize();

			this->tris[i]->tNormal = normal;

		}
		return TACore::TACORE_OK;
	}
	
}
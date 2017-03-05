#ifndef TA_TRIANGULAR_MESH_H
#define TA_TRIANGULAR_MESH_H

#include <vector>
#include <core/Defs.h>
#include <core/TAVector.h>
#include <core/PolygonMesh.h>

namespace TAShape
{
	class Triangle
	{
	public:
		int idx;								//< Index of the triangle: simply tris[idx] 
		int v1i, v2i, v3i;						//< Indices of the vertices of the triangle verts[v1i]-verts[v2i]-verts[v3i]					
		int e1, e2, e3;							//< Indices of the edges of the triangle
		TAVector tNormal;						//< Normal vector of the triangle
		Triangle(const int& i, const int& a, const int& b, const int& c) : idx(i), v1i(a), v2i(b), v3i(c) { e1 = -1; e2 = -1; e3 = -1; };
	};

	class Vertex
	{
	public:
		virtual ~Vertex() { TACORE_SAFE_DELETE_ARRAY(coords); }
	public:
		int idx;								//< Index of the vertex: simply verts[idx]
		float* coords;							//< coords[0] ~ x coord, ..

		std::vector< int > triList;				//< Indices of the trianges whose one vertex is this vertex
		std::vector< int > edgeList;			//< Indices of the edges whose one vertex is this vertex
		std::vector< int > vertList;			//< Indices of the vertices which are neighbor to this vertex

		bool interior;

		Vertex(const int& i, float* c) : idx(i), coords(c) {};
	};

	struct Edge
	{
	public:
		int idx;								//< Index of the edge: simply edges[idx]
		int v1i, v2i;							//< Indices of the vertices of the edgess		
		float length;							//< Length of the edge

		Edge(const int& i, const int& a, const int& b,const float& len) : idx(i), v1i(a), v2i(b), length(len) {};
	};

	//Simple TriangularMesh Class
	//Open Inventor shape provider uses this class as TriangularMesh
	// [Currently] Loading off and obj files are supported.
	// [TO DO: ] Write save method for off and obj files
	class TriangularMesh : public PolygonMesh
	{
	public:
		TriangularMesh();
		virtual ~TriangularMesh();
	public:
		std::vector< Triangle* > tris;			//< Pointers to triangles
		std::vector< Vertex* > verts;			//< Pointers to vertices
		std::vector< Edge* > edges;				//< Pointers to edges

	public:

		/**
		* @brief			Gets the polygon type of the mesh
		* @return			PolygonType is TA_TRIANGULAR.
		*/
		virtual PolygonType getPolygonType() const { return TA_TRIANGULAR; };

		/**
		* @brief			Clears the allocated buffers for tris, verts and edges
		* @return			TACORE_OK if successfully cleared.
		*/
		TACore::Result clear();

		/**
		* @brief			Loads the TriangularMesh..
		* @param			[in] fName Path to file.
		* @return			TACORE_OK successfully loaded.
		* @return			TACORE_INVALID_OPERATION if the extension is not supported
		* @return			TACORE_FILE_ERROR if the file cannot be opened
		* @return			TACORE_ERROR if there is an isolated vertex in the file
		*/
		TACore::Result load(const char* fName);

		/**
		* @brief			Loads the TriangularMesh by reading .off file.
		* @param			[in] fName Path to .off file.
		* @return			TACORE_OK successfully loaded.
		* @return			TACORE_FILE_ERROR if the file cannot be opened
		* @return			TACORE_ERROR if there is an isolated vertex in the file
		*/
		TACore::Result loadOff(const char* fName);

		/**
		* @brief			Loads the TriangularMesh by reading .obj file.
		* @param			[in] fName Path to .obj file.
		* @return			TACORE_OK successfully loaded.
		* @return			TACORE_FILE_ERROR if the file cannot be opened
		* @return			TACORE_ERROR if there is an isolated vertex in the file
		*/
		TACore::Result loadObj(const char* fName);

		/**
		* @brief			Creates a simple cumbe TriangularMesh
		* @param			[in] sl Size of cube edges
		* @return			TACORE_OK successfully created.
		*/
		TACore::Result createCube(float sl);

		/**
		* @brief			Adds a new vertex to the mesh by allocating a new space
		* @param			[in] c Coordinates of the vertex
		* @return			TACORE_OK successfully added.
		*/
		TACore::Result addVertex(float* c);

		/**
		* @brief			Adds a new triangle from the already added vertices
		*					New triangle is added to the triangle list of all vertices
		*					If there is not an edge between two of the vertices, a new edge is created and added
		* @param			[in] v1i, v2i, v3i Indices of the vertices
		* @return			TACORE_OK successfully added.
		*/
		TACore::Result addTriangle(int v1i, int v2i, int v3i);

		/**
		* @brief			Adds a new edge from the already added vertices
		*					New edge is added to the edge list of all vertices
		* @param			[in] v1i, v2i Indices of the vertices
		* @return			The index of the edge created
		*/
		int addEdge(int v1i, int v2i);

		/**
		* @brief			Tries to connect two vertices with their indexes
		*					If there is not an edge between two of the vertices, a new edge is created and added
		* @param			[in] v, w Indices of the vertices
		* @return			The index of the edge between v and w
		*/
		int makeVertsNeighbors(int v, int w);

		/**
		* @brief			Calculates euc distance between to vertices
		* @param			[in] v1, Pointer to a vertex
		* @param			[i] v2, Pointer to other vertexs
		* @return			Euc distance between two
		*/
		float eucDistanceBetween(Vertex *v1, Vertex *v2) const;

		/**
		* @brief			Calculates and assigns the normals for each triangle
		* @return			TACORE_OK successfully assigned.
		*/
		TACore::Result assignNormalsToTriangles();

		/**
		* @brief			Calculates the area of a triangle
		* @param			[in] t, index of the triangle whose area is being calculated
		* @return			Area of the triangle
		*/
		float calcAreaOfTriangle(int t) const;

		/**
		* @brief			Calculates the ring areas of all vertices inside the mesh
		*					Ring area of a vertex is the sum of areas of triangles that fan around it
		* @param			[out] ringAreas, Ring areas of all vertices inside the mesh
		* @return			void
		*/
		void calcRingAreasOfVertices(std::vector<double>& ringAreas) const;

		/**
		* @brief			Calculates the average edge length of the mesh
		* @return			Average edge length
		*/
		float calcAverageEdgeLength() const;

		/**
		* @brief			Calculates the total surface area
		* @return			Total surface area
		*/
		float calcTotalSurfaceArea() const;

		/**
		* @brief			Calculates the maximum euc distance between two vertices inside the mesh
		* @return			Maximum euc distance in the mesh
		*/
		float calcMaxEucDistanceBetweenTwoVertices() const;

		/**
		* @brief			Calculates the maximum area between three vertices inside the mesh
		* @return			Maximum area of three vertices in the mesh
		*/
		float calcMaxAreaBetweenThreeVertices() const;

		/**
		* @brief			Calculates the maximum volume of the tetrahedron between four vertices inside the mesh
		* @return			Maximum volume of tetrahedron between four vertices in the mesh
		*/
		float calcMaxVolumeOfTetrahedronBetweenFourVertices() const;
	};

}

#endif
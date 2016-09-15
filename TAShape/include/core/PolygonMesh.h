#ifndef TA_POLYGON_MESH_H
#define TA_POLYGON_MESH_H

namespace TAShape
{
	enum PolygonType
	{
		TA_TRIANGULAR
	};

	//Base class for polygon meshes
	class PolygonMesh
	{
	public:
		virtual ~PolygonMesh() {};
		PolygonMesh() {};
		PolygonMesh(const PolygonMesh& other) {}

		virtual PolygonType getPolygonType() const = 0;
	};
}

#endif
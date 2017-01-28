#ifndef OPEN_INVENTOR_SHAPE_PAINTER_H
#define OPEN_INVENTOR_SHAPE_PAINTER_H

#include <core/TriangularMesh.h>

class SoMaterial;
class SoSeparator;
class SoCoordinate3;
class SoIndexedFaceSet;

namespace TAShape
{
	class OIPainter
	{
		OIPainter();
		OIPainter(const OIPainter&);
	public:
		static SoSeparator* getShapeSep(TriangularMesh* mesh);
		static SoSeparator* getShapeSepWithClosedEdges(TriangularMesh* mesh, const std::vector<int>& edgeIds);
		static SoSeparator* getShapeSepWithVertexColors(TriangularMesh* mesh, const std::vector<std::vector< unsigned char > >& pColors);
		static SoSeparator* getShapeSepWithFaceColors(TriangularMesh* mesh, const std::vector<std::vector< unsigned char > >& pColors);
		static SoSeparator* getVertexSphereSep(Vertex *ver, float radius, SoMaterial* mat = NULL);
		/*static SoSeparator* getColorSep(TriangularMesh* mesh, unsigned int nColors, bool distColor);*/
		/*static SoSeparator* getVertexSphereSep(Vertex *ver, float radius, SoMaterial* mat);*/
		/*static SoSeparator* getLineBetweenTwoSep(TriangularMesh *mesh, Vertex *ver1, Vertex *ver2);*/
	private:
		static SoIndexedFaceSet* getShapeIndexedFaceSet(TriangularMesh* mesh);
		static SoCoordinate3* getShapeCoordinates(TriangularMesh* mesh);
	};
}

#endif
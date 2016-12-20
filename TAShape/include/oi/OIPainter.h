#ifndef OPEN_INVENTOR_SHAPE_PAINTER_H
#define OPEN_INVENTOR_SHAPE_PAINTER_H

#include <Inventor/nodes/SoSeparator.h>ss
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <core/TriangularMesh.h>

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
		/*static SoSeparator* getColorSep(TriangularMesh* mesh, unsigned int nColors, bool distColor);*/
		/*static SoSeparator* getVertexSphereSep(Vertex *ver, float radius, SoMaterial* mat);*/
		/*static SoSeparator* getLineBetweenTwoSep(TriangularMesh *mesh, Vertex *ver1, Vertex *ver2);*/
	private:
		static SoIndexedFaceSet* getShapeIndexedFaceSet(TriangularMesh* mesh);
		static SoCoordinate3* getShapeCoordinates(TriangularMesh* mesh);
	};
}

#endif
#include <oi/OIPainter.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>

namespace TAShape
{
	SoSeparator* OIPainter::getShapeSep(TriangularMesh* mesh)
	{
		SoSeparator* sep = new SoSeparator();

		//material
		SoMaterial* mat = new SoMaterial();
		mat->diffuseColor.setValue(1, 1, 0);
		sep->addChild(mat);

		sep->addChild(getShapeCoordinates(mesh));
		sep->addChild(getShapeIndexedFaceSet(mesh));

		return sep;
	}

	SoSeparator* OIPainter::getShapeSepWithClosedEdges(TriangularMesh* mesh, const std::vector<int>& edgeIds)
	{
		std::vector<std::pair <int, int> > verticesOfClosedPath;
		for (size_t i = 0; i < edgeIds.size(); i++)
		{
			const int ev1 = mesh->edges[edgeIds[i]]->v1i;
			const int ev2 = mesh->edges[edgeIds[i]]->v2i;
			verticesOfClosedPath.push_back(std::make_pair(ev1, ev2));
		}

		SoDrawStyle *dstyle = new SoDrawStyle;
		dstyle->lineWidth = 5;

		size_t numOfVertices = verticesOfClosedPath.size();
		if (numOfVertices <= 1) return NULL;
		SbVec3f *crds = new SbVec3f[numOfVertices];

		int searchedVertex = verticesOfClosedPath[0].first;
		for (size_t i = 0; i < numOfVertices; i++) {
			int otherVertex = -1;
			int foundVertexEdgeIndex = -1;
			for (size_t j = 0; j < verticesOfClosedPath.size(); j++)
			{
				if (verticesOfClosedPath[j].first == searchedVertex)
				{
					otherVertex = verticesOfClosedPath[j].second;
					foundVertexEdgeIndex = j;
					break;
				}
				if (verticesOfClosedPath[j].second == searchedVertex)
				{
					otherVertex = verticesOfClosedPath[j].first;
					foundVertexEdgeIndex = j;
					break;
				}
			}

			crds[i][0] = mesh->verts[searchedVertex]->coords[0];
			crds[i][1] = mesh->verts[searchedVertex]->coords[1];
			crds[i][2] = mesh->verts[searchedVertex]->coords[2];

			if (otherVertex == -1 || foundVertexEdgeIndex == -1)
			{
				return NULL;
			}

			verticesOfClosedPath.erase(verticesOfClosedPath.begin() + foundVertexEdgeIndex);
			searchedVertex = otherVertex;
		}

		SoVertexProperty *vprop = new SoVertexProperty();
		vprop->vertex.setValues(0, numOfVertices, crds);
		vprop->orderedRGBA.setValue(0xFF0000FF);

		static int numPoints[1];
		numPoints[0] = numOfVertices;

		SoLineSet *lines = new SoLineSet;
		lines->numVertices.setValues(0, 1, numPoints);

		lines->vertexProperty.setValue(vprop);
		SoSeparator* sep = new SoSeparator();
		sep->addChild(dstyle);
		sep->addChild(lines);

		return sep;
	}

	SoSeparator* OIPainter::getShapeSepWithVertexColors(TriangularMesh* mesh, const std::vector<std::vector< unsigned char > >& pColors)
	{
		if (pColors.size() != 3)
		{
			std::cerr << "RGB Colors must be provided!" << std::endl;
			return NULL;
		}
		if (pColors[0].size() != mesh->verts.size())
		{
			std::cerr << "Number of colors and vertex must be equal!" << std::endl;
			return NULL;
		}

		SoSeparator* sep = new SoSeparator();

		sep->addChild(getShapeCoordinates(mesh));

		SoVertexProperty* vProp = new SoVertexProperty();
		vProp->materialBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);
		for (int v = 0; v < (int)mesh->verts.size(); v++) 
		{
			const uint32_t r = pColors[0][v] << 24;
			const uint32_t g = pColors[1][v] << 16;
			const uint32_t b = pColors[2][v] << 8;
		
			const uint32_t colValue = ((r | g) | b) | 0xFF;
			vProp->orderedRGBA.set1Value(v, colValue);
		}
		sep->addChild(vProp);

		sep->addChild(getShapeIndexedFaceSet(mesh));
		return sep;
	}

	SoCoordinate3* OIPainter::getShapeCoordinates(TriangularMesh* mesh)
	{
		SoCoordinate3* coords = new SoCoordinate3();

		for (int v = 0; v < (int)mesh->verts.size(); v++)
			coords->point.set1Value(v, mesh->verts[v]->coords[0], mesh->verts[v]->coords[1], mesh->verts[v]->coords[2]);
		return coords;
	}

	SoIndexedFaceSet* OIPainter::getShapeIndexedFaceSet(TriangularMesh* mesh)
	{
		SoIndexedFaceSet* faceSet = new SoIndexedFaceSet();
		int nt = 0;
		for (int t = 0; t < (int)mesh->tris.size(); t++)
		{
			faceSet->coordIndex.set1Value(0 + 4 * nt, mesh->tris[t]->v1i);
			faceSet->coordIndex.set1Value(1 + 4 * nt, mesh->tris[t]->v2i);
			faceSet->coordIndex.set1Value(2 + 4 * nt, mesh->tris[t]->v3i);
			faceSet->coordIndex.set1Value(3 + 4 * nt++, -1);
		}
		return faceSet;
	}

	SoSeparator* OIPainter::getVertexSphereSep(Vertex *ver, float radius, SoMaterial* mat)
	{
		SoMaterial* localMat = NULL;
		//material
		if (mat == NULL)
		{
			localMat = new SoMaterial();
			localMat->diffuseColor.setValue(1, 1, 0);
		}
		else
		{
			localMat = mat;
		}

		SoSeparator* sep = new SoSeparator();
		SoSphere *sphere = new SoSphere();
		SoTransform *tr = new SoTransform();
		sphere->radius = radius;
		tr->translation = SbVec3f(ver->coords[0], ver->coords[1], ver->coords[2]);
		sep->addChild(localMat);
		sep->addChild(tr);
		sep->addChild(sphere);
	
		return sep;
	}


	//SoSeparator* OIPainter::getColorSep(PolygonMesh* mesh, unsigned int nColors, bool distColor)
	//{
	//	SoSeparator* sep = new SoSeparator();

	//	//transformation
	//	//not needed
	//	//material
	//	uint32_t *colArray = new uint32_t[nColors];
	//	for (unsigned int i = 0; i < nColors; i++) {
	//		SoMaterial* mat = giveRandomMaterial();
	//		colArray[i] = giveRandom24bit();
	//	}

	//	//shape
	//	SoCoordinate3* coords = new SoCoordinate3();

	//	for (int v = 0; v < (int)mesh->verts.size(); v++)
	//		coords->point.set1Value(v, mesh->verts[v]->coords[0], mesh->verts[v]->coords[1], mesh->verts[v]->coords[2]);
	//	sep->addChild(coords);

	//	SoVertexProperty* vProp = new SoVertexProperty();
	//	vProp->materialBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);
	//	for (int v = 0; v < (int)mesh->verts.size(); v++) {
	//		if (distColor)
	//			vProp->orderedRGBA.set1Value(v, colArray[unsigned int(mesh->verts[v]->avgDistance * nColors)]);
	//		else
	//			vProp->orderedRGBA.set1Value(v, colArray[unsigned int(mesh->verts[v]->curvature * nColors)]);
	//	}
	//	sep->addChild(vProp);

	//	SoIndexedFaceSet* faceSet = new SoIndexedFaceSet();
	//	int nt = 0;
	//	for (int t = 0; t < (int)mesh->tris.size(); t++)
	//	{
	//		faceSet->coordIndex.set1Value(0 + 4 * nt, mesh->tris[t]->v1i);
	//		faceSet->coordIndex.set1Value(1 + 4 * nt, mesh->tris[t]->v2i);
	//		faceSet->coordIndex.set1Value(2 + 4 * nt, mesh->tris[t]->v3i);
	//		faceSet->coordIndex.set1Value(3 + 4 * nt++, -1);
	//	}
	//	sep->addChild(faceSet);

	//	return sep;
	//}

	//SoSeparator* OIPainter::getLineBetweenTwoSep(PolygonMesh *mesh, Vertex *ver1, Vertex *ver2)
	//{
	//	SoDrawStyle *dstyle = new SoDrawStyle;
	//	dstyle->lineWidth = 10;

	//	unsigned int numOfVertices = 0;
	//	Vertex *temp = ver2;
	//	while (temp->idx != ver1->idx) {
	//		numOfVertices += 1;
	//		temp = temp->neighVertex;
	//	}
	//	numOfVertices += 1;
	//	if (numOfVertices <= 1) return NULL;
	//	SbVec3f *crds = new SbVec3f[numOfVertices];

	//	temp = ver2;
	//	for (unsigned int i = 0; i < numOfVertices; i++) {
	//		crds[i][0] = temp->coords[0];
	//		crds[i][1] = temp->coords[1];
	//		crds[i][2] = temp->coords[2];
	//		temp = temp->neighVertex;

	//	}

	//	if (temp) {
	//		std::cerr << "Path is not valid" << std::endl;
	//		if (crds) {
	//			delete[]crds;
	//		}
	//		return NULL;
	//	}
	//	SoVertexProperty *vprop = new SoVertexProperty();
	//	vprop->vertex.setValues(0, numOfVertices, crds);
	//	vprop->orderedRGBA.setValue(0xFF0000FF);

	//	static int numPoints[1];
	//	numPoints[0] = numOfVertices;

	//	SoLineSet *lines = new SoLineSet;
	//	lines->numVertices.setValues(0, 1, numPoints);

	//	lines->vertexProperty.setValue(vprop);
	//	SoSeparator* sep = new SoSeparator();
	//	sep->addChild(dstyle);
	//	sep->addChild(lines);

	//	return sep;
	//}
}
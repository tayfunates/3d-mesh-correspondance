#ifndef THREE_DIMENSIONAL_OI_SHAPE_H
#define THREE_DIMENSIONAL_OI_SHAPE_H

#include <core/CoreDefs.h>
#include <core/ThreeDimShape.h>
#include <core/TriangularMesh.h>

namespace TAShape
{
	class ThreeDimOIShape : public ThreeDimShape
	{
	public:
		virtual ~ThreeDimOIShape();

		virtual TACore::Result load(const std::string& pPath) OVERRIDE;
		virtual TACore::Result save(const std::string& pPath) OVERRIDE;
		virtual TACore::Result clear() OVERRIDE;
		virtual bool empty() const OVERRIDE;
		virtual TACore::Result show() OVERRIDE;

		virtual TACore::Result showClosedEdges(const std::vector<int>& pEdgeIds);
		virtual TACore::Result showClosedEdges(const std::vector< std::vector<int> >& pClosedWaves);

		virtual TACore::Result showVertexColors(const std::vector<double>& pVertMagnitudes, const int& pSpecialVertexId = -1);

		TAShape::PolygonMesh* getShape() const;

	private:
		TAShape::TriangularMesh *m_p3DShape;
	};
}

#endif

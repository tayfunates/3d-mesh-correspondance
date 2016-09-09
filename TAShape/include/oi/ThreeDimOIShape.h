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
		virtual TACore::Result load(const std::string& pPath) OVERRIDE;
		virtual TACore::Result save(const std::string& pPath) OVERRIDE;
		virtual TACore::Result clear() OVERRIDE;
		virtual bool empty() const OVERRIDE;
		virtual TACore::Result show() OVERRIDE;

	private:
		TAShape::TriangularMesh m_3DShape;
	};
}

#endif

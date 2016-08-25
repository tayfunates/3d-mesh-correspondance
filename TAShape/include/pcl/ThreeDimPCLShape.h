#ifndef THREE_DIMENSIONAL_PCL_SHAPE_H
#define THREE_DIMENSIONAL_PCL_SHAPE_H

#include <pcl/PolygonMesh.h>
#include <core/CoreDefs.h>
#include <core/ThreeDimShape.h>

namespace TAShape
{
	class ThreeDimPCLShape : public ThreeDimShape
	{
	public:
		virtual TACore::Result load(const std::string& pPath) OVERRIDE;
		virtual TACore::Result save(const std::string& pPath) OVERRIDE;
		virtual TACore::Result clear() OVERRIDE;
		virtual TACore::Result show() OVERRIDE;

	private:
		pcl::PolygonMesh m_3DShape;
	};
}

#endif

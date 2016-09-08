#ifndef THREE_DIMENSIONAL_PCL_SHAPE_H
#define THREE_DIMENSIONAL_PCL_SHAPE_H

#include <pcl/PolygonMesh.h>
#include <core/CoreDefs.h>
#include <core/ThreeDimShape.h>

namespace TAShape
{
	//Basic PCL typedefs
	typedef ::pcl::Vertices PCL_POLYGON;
	typedef std::vector< PCL_POLYGON > PCL_POLYGON_LIST;
	typedef ::pcl::PCLPointCloud2 PCL_POINT_CLOUD;

	class ThreeDimPCLShape : public ThreeDimShape
	{
	public:
		virtual ~ThreeDimPCLShape() { clear(); }

		/**
		* @brief			Loads the shape from a file provided by a path using pcl api
		*					[Currently] used extensions: .obj
		* @param			[in] pPath Path to shape file.
		* @return			TACORE_OK successfully loaded.
		* @return			TACORE_ERROR if the load result from pcl returns an error
		*/
		virtual TACore::Result load(const std::string& pPath) OVERRIDE;

		/**
		* @brief			Saves the shape to a file provided by a path using pcl api
		*					[Currently] used extensions: .obj
		* @param			[in] pPath Path to save file.
		* @return			TACORE_OK successfully loaded.
		* @return			TACORE_ERROR if the save result from pcl returns an error
		*/
		virtual TACore::Result save(const std::string& pPath) OVERRIDE;

		/**
		* @brief			Clears the shape.
		*					[Currently] this method simply returns ok
		*					[TO DO] Check for memory leaks
		* @return			TACORE_OK successfully cleared.
		*/
		virtual TACore::Result clear() OVERRIDE;

		/**
		* @brief			Shows the shape in the visualisation tool provided by pcl
		*					This method is intended to be used in run time.
		*					Specific user interrupts may be required to continue the execution
		*					[Currently] the shape and the camera are far from each other. Fix it
		* @return			TACORE_OK successfully showed.
		* @return			TACORE_INVALID_OPERATION if the mesh cannot be added to the viewer.
		*/
		virtual TACore::Result show() OVERRIDE;


		virtual PCL_POLYGON_LIST& getPolygons();
		virtual PCL_POINT_CLOUD& getCloud();


	private:
		pcl::PolygonMesh m_3DShape;
	};
}

#endif

#include <pcl/io/file_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/ThreeDimPCLShape.h>

namespace TAShape
{
	TACore::Result ThreeDimPCLShape::load(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");

		//Clear the all shape
		clear();

		int loadRes = pcl::io::load(pPath, this->m_3DShape);
		if (loadRes < 0)
		{
			return TACore::TACORE_ERROR;
		}
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimPCLShape::save(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");
		int saveRes = pcl::io::save(pPath, this->m_3DShape);
		if (saveRes < 0)
		{
			return TACore::TACORE_ERROR;
		}
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimPCLShape::clear()
	{
		return TACore::TACORE_OK;
	}

	bool ThreeDimPCLShape::empty() const
	{
		//The shape is empty if the width or the height of the point cloud is empty
		return this->m_3DShape.cloud.width == 0 || this->m_3DShape.cloud.height == 0;
	}

	TACore::Result ThreeDimPCLShape::show()
	{
		boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
		viewer->setBackgroundColor(0, 0, 0);
		bool bAddMesh = viewer->addPolygonMesh(this->m_3DShape, "meshes", 0);
		if (!bAddMesh)
		{
			viewer->close();
			return TACore::TACORE_INVALID_OPERATION;
		}

		viewer->addCoordinateSystem(1.0);
		viewer->initCameraParameters();
		while (!viewer->wasStopped()){
			viewer->spinOnce(100);
			boost::this_thread::sleep(boost::posix_time::microseconds(100000));
		}
		viewer->close();
		return TACore::TACORE_OK;
	}

	TAShape::TriangularMesh* ThreeDimPCLShape::toTriangularMesh(const ThreeDimPCLShape& pclShape)
	{
		if (pclShape.empty())
		{
			return NULL;
		}
		if (pclShape.m_3DShape.polygons.size() == 0)
		{
			//No polygon exists
			return NULL;
		}
		if (pclShape.m_3DShape.polygons[0].vertices.size() != 3)
		{
			//No triangular mesh
			return NULL;
		}

		pcl::PointCloud< pcl::PointXYZ > pclPointCloud1; //PCL has two types of point clouds
		pcl::fromPCLPointCloud2(pclShape.m_3DShape.cloud, pclPointCloud1);

		TriangularMesh *res = new TAShape::TriangularMesh;
		
		for (size_t i = 0; i < pclPointCloud1.points.size(); i++)
		{
			float *coords = new float[3];
			coords[0] = pclPointCloud1.points[i].x;
			coords[1] = pclPointCloud1.points[i].y;
			coords[2] = pclPointCloud1.points[i].z;

			res->addVertex(coords); //No duplicate check
		}


		for (size_t i = 0; i < pclShape.m_3DShape.polygons.size(); i++)
		{
			const PCL_POLYGON& polygon = pclShape.m_3DShape.polygons[i];
			res->addTriangle(polygon.vertices[0], polygon.vertices[1], polygon.vertices[2]);
		}

		return res;
	}

	ThreeDimPCLShape* ThreeDimPCLShape::fromTriangularMesh(const TAShape::TriangularMesh& taShape)
	{
		if (taShape.verts.empty())
		{
			return NULL;
		}
		if (taShape.tris.empty())
		{
			//No polygon exists
			return NULL;
		}

		pcl::PointCloud< pcl::PointXYZ > pclPointCloud1; //PCL has two types of point clouds
		for (size_t i = 0; i < taShape.verts.size(); i++)
		{
			pcl::PointXYZ pclPoint;
			pclPoint.x = taShape.verts[i]->coords[0];
			pclPoint.y = taShape.verts[i]->coords[1];
			pclPoint.z = taShape.verts[i]->coords[2];

			pclPointCloud1.points.push_back(pclPoint);
		}

		ThreeDimPCLShape *res = new ThreeDimPCLShape;
		pcl::toPCLPointCloud2(pclPointCloud1, res->m_3DShape.cloud);

		for (size_t i = 0; i < taShape.tris.size(); i++)
		{
			PCL_POLYGON polygon;
			polygon.vertices.push_back(taShape.tris[i]->v1i);
			polygon.vertices.push_back(taShape.tris[i]->v2i);
			polygon.vertices.push_back(taShape.tris[i]->v3i);

			res->m_3DShape.polygons.push_back(polygon);
		}
		
		return res;
	}
}
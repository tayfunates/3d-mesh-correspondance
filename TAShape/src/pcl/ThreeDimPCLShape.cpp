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
}
#include <oi/ThreeDimOIShape.h>
#include <oi/OIPainter.h>
#include <core/PathUtil.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <ctime>

namespace TAShape
{
	ThreeDimOIShape::~ThreeDimOIShape()
	{
		clear();
	}

	TACore::Result ThreeDimOIShape::load(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");

		//Clear the all shape
		clear();
		this->m_p3DShape = new TriangularMesh();

		std::string fileExt = TACore::PathUtil::getExtension(pPath);
		if (fileExt == "off")
		{
			return this->m_p3DShape->loadOff(pPath.c_str());
		}
		else if (fileExt == "obj")
		{
			return this->m_p3DShape->loadObj(pPath.c_str());
		}
		else
		{
			std::cerr << "unsupported extension type" << std::endl;
			clear();
			return TACore::TACORE_ERROR;
		}

		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::save(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");
		return TACore::TACORE_OK;
	}

	bool ThreeDimOIShape::empty() const
	{
		return this->m_p3DShape == NULL || this->m_p3DShape->verts.size() == 0;
	}

	TACore::Result ThreeDimOIShape::clear()
	{
		TACORE_SAFE_DELETE(this->m_p3DShape);
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::show()
	{
		srand(time(NULL));
		HWND window = SoWin::init("TAShapeTest.exe");
		SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);
		SoSeparator * root = new SoSeparator();
		root->ref();
		SoSeparator* generalShape = OIPainter::getShapeSep(this->m_p3DShape);

		root->addChild(generalShape);

		viewer->setSceneGraph(root);
		viewer->show();

		SoWin::show(window);
		SoWin::mainLoop();
		
		root->unref();
		TACORE_SAFE_DELETE(viewer);

		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::showClosedEdges(const std::vector<int>& pEdgeIds)
	{
		srand(time(NULL));
		HWND window = SoWin::init("TAShapeTest.exe");
		SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);
		SoSeparator * root = new SoSeparator();
		root->ref();
		SoSeparator* closedEdges = OIPainter::getShapeSepWithClosedEdges(this->m_p3DShape, pEdgeIds);
		SoSeparator* generalShape = OIPainter::getShapeSep(this->m_p3DShape);

		root->addChild(generalShape);
		root->addChild(closedEdges);

		viewer->setSceneGraph(root);
		viewer->show();

		SoWin::show(window);
		SoWin::mainLoop();

		root->unref();
		TACORE_SAFE_DELETE(viewer);

		return TACore::TACORE_OK;
	}

	TAShape::PolygonMesh* ThreeDimOIShape::getShape() const
	{
		return this->m_p3DShape;
	}
}
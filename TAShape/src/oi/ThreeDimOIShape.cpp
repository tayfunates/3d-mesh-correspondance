#include <oi/ThreeDimOIShape.h>
#include <oi/OIPainter.h>
#include <core/PathUtil.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <ctime>

namespace TAShape
{
	TACore::Result ThreeDimOIShape::load(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");

		//Clear the all shape
		clear();

		std::string fileExt = TACore::PathUtil::getExtension(pPath);
		if (fileExt == "off")
		{
			return this->m_3DShape.loadOff(pPath.c_str());
		}
		else if (fileExt == "obj")
		{
			return this->m_3DShape.loadObj(pPath.c_str());
		}

		std::cerr << "unsupported extension type" << std::endl;
		return TACore::TACORE_ERROR;
	}

	TACore::Result ThreeDimOIShape::save(const std::string& pPath)
	{
		TACORE_CHECK_ARGS(pPath != "");
		return TACore::TACORE_OK;
	}

	bool ThreeDimOIShape::empty() const
	{
		return this->m_3DShape.verts.size() == 0;
	}

	TACore::Result ThreeDimOIShape::clear()
	{
		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::show()
	{
		srand(time(NULL));
		HWND window = SoWin::init("TAShapeTest.exe");
		SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);
		SoSeparator * root = new SoSeparator();
		root->ref();
		SoSeparator* generalShape = OIPainter::getShapeSep(&this->m_3DShape);

		root->addChild(generalShape);

		viewer->setSceneGraph(root);
		viewer->show();

		SoWin::show(window);
		SoWin::mainLoop();
		
		root->unref();
		TACORE_SAFE_DELETE(viewer);

		return TACore::TACORE_OK;
	}
}
#include <oi/ThreeDimOIShape.h>
#include <oi/OIPainter.h>
#include <core/PathUtil.h>
#include <core/ColorPalette.h>
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

		if(generalShape) root->addChild(generalShape);

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
		std::vector< std::vector<int> > multipleClosedWaves;
		multipleClosedWaves.push_back(pEdgeIds);
		return showClosedEdges(multipleClosedWaves);
	}

	TACore::Result ThreeDimOIShape::showClosedEdges(const std::vector< std::vector<int> >& pClosedWaves)
	{
		srand(time(NULL));
		HWND window = SoWin::init("TAShapeTest.exe");
		SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);
		SoSeparator * root = new SoSeparator();
		root->ref();
		for (size_t i = 0; i < pClosedWaves.size(); i++)
		{
			SoSeparator* closedEdges = OIPainter::getShapeSepWithClosedEdges(this->m_p3DShape, pClosedWaves[i]);
			if (closedEdges) root->addChild(closedEdges);
		}
		
		SoSeparator* generalShape = OIPainter::getShapeSep(this->m_p3DShape);
		if (generalShape) root->addChild(generalShape);
		
		viewer->setSceneGraph(root);
		viewer->show();

		SoWin::show(window);
		SoWin::mainLoop();

		root->unref();
		TACORE_SAFE_DELETE(viewer);

		return TACore::TACORE_OK;
	}

	TACore::Result ThreeDimOIShape::showVertexColors(const std::vector<double>& pVertMagnitudes)
	{
		//If magnitude vector size is not equal to the vertex size
		if (pVertMagnitudes.size() != this->m_p3DShape->verts.size())
		{
			return TACore::TACORE_BAD_ARGS;
		}
		TACore::ColorPalette colorPalette;


		std::vector< std::vector< unsigned char > > colorsFromMagnitudes(3);
		colorsFromMagnitudes[0] = std::vector<unsigned char>(pVertMagnitudes.size());
		colorsFromMagnitudes[1] = std::vector<unsigned char>(pVertMagnitudes.size());
		colorsFromMagnitudes[2] = std::vector<unsigned char>(pVertMagnitudes.size());
		for (size_t i = 0; i < pVertMagnitudes.size(); i++)
		{
			unsigned char r, g, b;
			colorPalette.getColor(pVertMagnitudes[i], r, g, b);
			colorsFromMagnitudes[0][i] = r;
			colorsFromMagnitudes[1][i] = g;
			colorsFromMagnitudes[2][i] = b;
		}

		srand(time(NULL));
		HWND window = SoWin::init("TAShapeTest.exe");
		SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);
		SoSeparator * root = new SoSeparator();
		root->ref();

		SoSeparator* generalShape = OIPainter::getShapeSepWithVertexColors(this->m_p3DShape, colorsFromMagnitudes);
		if (generalShape) root->addChild(generalShape);

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
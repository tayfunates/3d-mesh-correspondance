#include <iostream>
#include <core/TriangularMesh.h>
#include <core/CommandLineParser.h>
#include <core/Timer.h>

#include <oi/ThreeDimOIShape.h>

//Descriptors
#include "AvgGeodesicDistance.h"
#include "IntrinsicWaveDesc.h"

//Extractors
#include "OnEdgeAvgGeoDistExtraction.h"
#include "IntrinsicWaveDescExtraction.h"



using namespace TACore;
using namespace TAShape;

int mainRet(int ret, std::string message)
{
	std::cout << "Message: " << message << std::endl;
#ifdef TACORE_IS_WINDOWS
	std::cout << "Press Enter to Exit The App..." << std::endl;
	system("pause");
#endif
	return ret;
}

int main(int argc, char* argv[])
{
	// Parse args
	TACore::ArgParser parser("TAShapeTest", "Loads, Saves, Shows different types of shapes");
	parser.addArg("input", "", true, 1, "", "Input shape file to be loaded");
	parser.addArg("output", "", true, 1, "", "Output shape file to be saved");

	if (!parser.parseCommandLine(argc, argv))
	{
		mainRet(-1, "Command line parameters cannot be parsed correctly");
	}

	std::string inputFile = parser.get("input");
	std::string outputFile = parser.get("output");
	 
	ThreeDimOIShape mesh;

	if (mesh.load(inputFile.c_str()) != TACORE_OK)
	{
		return mainRet(1, "Mesh cannot be loaded correctly");
	}
	mesh.show();
	//ThreeDimPCLShape *pclShape = ThreeDimPCLShape::fromTriangularMesh(*triMesh);
	//pclShape->show();

	//TAFeaExt::IntrinsicWaveDescExtraction waveDescExtractor;
	//waveDescExtractor.setMaxGeodesicRadius(100.0f);

	////TACore::Timer timer;
	////std::cout << "Geodesic Distances for all vertices are being calculated" << std::endl;

	//for (size_t v = 0; v < triMesh->verts.size(); v++)
	//{
	//	LocalFeaturePtr localFeaturePtr;
	//	waveDescExtractor.extract(triMesh, v, localFeaturePtr);
	//	//std::cout << "Average geodesic distance of vertex: " << v << " is " << ((AvgGeodesicDistance*)(localFeaturePtr.get()))->m_distance << std::endl;

	//	break;
	//}
	///*std::cout << "Total time taken: " << timer.seconds() << std::endl;*/

	//TACORE_SAFE_DELETE(triMesh);

	return mainRet(1, "Main Test Successfully Ended");
}
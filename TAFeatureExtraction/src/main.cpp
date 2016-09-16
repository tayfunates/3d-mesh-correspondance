#include <iostream>
#include <core/TriangularMesh.h>
#include <core/CommandLineParser.h>
#include "AvgGeodesicDistance.h"
#include "OnEdgeAvgGeoDistExtraction.h"


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

	TriangularMesh *triMesh = new TriangularMesh;
	if (triMesh->load(inputFile.c_str()) != TACORE_OK)
	{
		return mainRet(1, "Mesh cannot be loaded correctly");
	}

	TAFeaExt::OnEdgeAvgGeoDistExtraction avgGeoExtractor;
	

	for (size_t v = 0; v < triMesh->verts.size(); v++)
	{
		TAFea::LocalFeature *distFea = NULL;
		avgGeoExtractor.extract(triMesh, v, &distFea);
		std::cout << "Average geodesic distance of vertex: " << v << " is " << ((AvgGeodesicDistance*)(distFea))->m_distance << std::endl;
		TACORE_SAFE_DELETE(distFea);
	}
	TACORE_SAFE_DELETE(triMesh);

	return mainRet(1, "Main Test Successfully Ended");
}
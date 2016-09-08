#include <pcl/ThreeDimPCLShape.h>
#include <oi/ThreeDimOIShape.h>
#include <iostream>
#include <core/CommandLineParser.h>


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

	ThreeDimPCLShape pclShape1;
	pclShape1.load(inputFile);
	pclShape1.save(outputFile);
	pclShape1.show();

	std::cout << "How many polygons1: " << pclShape1.getPolygons().size() << std::endl;
	std::cout << "101th Polygon vertex indices1: " << pclShape1.getPolygons()[100] << std::endl;

	std::cout << "Width1: " << pclShape1.getCloud().width << " Height1: " << pclShape1.getCloud().height << std::endl;
		 
	ThreeDimPCLShape pclShape2;
	pclShape2.load(outputFile);
	pclShape2.show();

	std::cout << "How many polygons2: " << pclShape1.getPolygons().size() << std::endl;
	std::cout << "101th Polygon vertex indices2: " << pclShape1.getPolygons()[100] << std::endl;
	std::cout << "Width2: " << pclShape1.getCloud().width << " Height2: " << pclShape1.getCloud().height << std::endl;

	return mainRet(1, "Main Test Successfully Ended");
}
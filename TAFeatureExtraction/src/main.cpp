#include <iostream>
#include <core/TriangularMesh.h>
#include <core/CommandLineParser.h>
#include <core/Timer.h>

#include <oi/ThreeDimOIShape.h>

//Descriptors
#include "AvgGeodesicDistance.h"
#include "IntrinsicWaveDesc.h"
#include "HeatKernelSignatureDesc.h"

//Extractors
#include "OnEdgeAvgGeoDistExtraction.h"
#include "IntrinsicWaveDescExtraction.h"
#include "HKSDescExtraction.h"

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

int HKSExtractionTestAPP(int argc, char* argv[])
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

	TriangularMesh* triMesh = (TriangularMesh*)(mesh.getShape());

	TAFeaExt::HKSDescExtraction hksExtractor;
	hksExtractor.setNumberOfEigenVals(50);
	hksExtractor.setMinTimeVal(1.0f);
	hksExtractor.setMaxTimeVal(25.0f);

	std::vector<double> distances1;
	std::vector<LocalFeaturePtr> feas1;
	hksExtractor.extract(triMesh, feas1);

	HeatKernelSignatureDesc* hksV0 = (HeatKernelSignatureDesc*)(feas1[0].get());
	for (size_t v = 0; v < feas1.size(); v++)
	{
		HeatKernelSignatureDesc* hksVV = (HeatKernelSignatureDesc*)(feas1[v].get());
		const double l2Distance = HeatKernelSignatureDesc::L2Distance(*hksVV, *hksV0);
		distances1.push_back(l2Distance);
	}

	const double maxDistance1 = *std::max_element(distances1.begin(), distances1.end());
	const double minDistance1 = *std::min_element(distances1.begin(), distances1.end());
	std::cout << minDistance1 << " " <<  maxDistance1 << std::endl;
	for (size_t i = 0; i < distances1.size(); i++)
	{
		distances1[i] = distances1[i] / maxDistance1;
	}

	mesh.showVertexColors(distances1);
	return mainRet(1, "Main Test Successfully Ended");
}

int IntrinsicWaveExtractionTestAPP(int argc, char* argv[])
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

	TriangularMesh* triMesh = (TriangularMesh*)(mesh.getShape());

	TAFeaExt::IntrinsicWaveDescExtraction waveDescExtractor;
	waveDescExtractor.setMaxGeodesicRadius(1000.0f);
	waveDescExtractor.setNumberOfWavesSampled(10);


	for (size_t v = 0; v < triMesh->verts.size(); v++)
	{
		LocalFeaturePtr localFeaturePtr;
		waveDescExtractor.extract(triMesh, v, localFeaturePtr);

		break;
	}

	mesh.showClosedEdges(waveDescExtractor.waves);

	return mainRet(1, "Main Test Successfully Ended");
}

int main(int argc, char* argv[])
{
	/*return IntrinsicWaveExtractionTestAPP(argc, argv);*/
	return HKSExtractionTestAPP(argc, argv);
}
#include <iostream>
#include <core/TAMatrix.h>
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
#include "GeodesicDistanceMatrixExtraction.h"
#include "PatchBasedPerVertexFeatureExtraction.h"
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

int GlobalDescForLocalPatchesTestApp(int argc, char* argv[])
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

	//TAFeaExt::PatchBasedPerVertexFeatureExtraction patchBasedExtractor;
	//patchBasedExtractor.setMinGeodesicDistance(40.f);
	//patchBasedExtractor.setMaxGeodesicDistance(100.0f);
	//patchBasedExtractor.setNumberOfPatches(10);

	//std::vector<double> distances;
	//std::vector<LocalFeaturePtr> feas;
	//patchBasedExtractor.extract(triMesh, feas);

	//HeatKernelSignatureDesc* hksV0 = (HeatKernelSignatureDesc*)(feas[126].get());
	//for (size_t v = 0; v < feas.size(); v++)
	//{
	//	HeatKernelSignatureDesc* hksVV = (HeatKernelSignatureDesc*)(feas[v].get());
	//	const double l2Distance = HeatKernelSignatureDesc::L2Distance(*hksVV, *hksV0);
	//	distances.push_back(l2Distance);
	//}

	//const double maxDistance = *std::max_element(distances.begin(), distances.end());
	//const double minDistance = *std::min_element(distances.begin(), distances.end());

	//for (size_t i = 0; i < distances.size(); i++)
	//{
	//	distances[i] = (distances[i] - minDistance) / (maxDistance - minDistance);
	//}

	//mesh.showVertexColors(distances, 126);
	return mainRet(1, "Main Test Successfully Ended");
}

int OnEdgeAvgDistExtractionTestApp(int argc, char* argv[])
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

	TAFeaExt::OnEdgeAvgGeoDistExtraction avgGeoExtractor;
	std::vector<LocalFeaturePtr> feas;
	avgGeoExtractor.extract(triMesh, feas);

	std::vector<double> distances(feas.size());

	AvgGeodesicDistance* avgGeoDistFeaPtr = (AvgGeodesicDistance*)(feas[126].get());
	const float referenceAvgGeoDistance = avgGeoDistFeaPtr->m_distance;
	for (size_t v = 0; v < feas.size(); v++)
	{
		AvgGeodesicDistance* avgGeoDistFeaPtr = (AvgGeodesicDistance*)(feas[v].get());
		const float queryAvgGeoDistance = avgGeoDistFeaPtr->m_distance;
		const float diffQueryReference = referenceAvgGeoDistance - queryAvgGeoDistance;
		distances[v] = sqrt(diffQueryReference * diffQueryReference);
	}

	const double maxDistance = *std::max_element(distances.begin(), distances.end());
	const double minDistance = *std::min_element(distances.begin(), distances.end());

	for (size_t i = 0; i < distances.size(); i++)
	{
		distances[i] = (distances[i] - minDistance) / (maxDistance - minDistance);
	}

	mesh.showVertexColors(distances, 126);
	return mainRet(1, "Main Test Successfully Ended");
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
	
	std::cout << "Total Surface Area Of the Mesh: " << triMesh->calcTotalSurfaceArea() << std::endl;

	TAFeaExt::HKSDescExtraction hksExtractor;
	hksExtractor.setNumberOfEigenVals(100);
	hksExtractor.setNumberOfTimeSamples(300);
	hksExtractor.setUseEigenValuesForTimeBoundaries(true);
	hksExtractor.setTypeOfLaplacian(TAFeaExt::HKSDescExtraction::DISCRETE_LAPLACIAN);

	std::vector<double> distances;
	std::vector<LocalFeaturePtr> feas;
	hksExtractor.extract(triMesh, feas);

	HeatKernelSignatureDesc* hksV0 = (HeatKernelSignatureDesc*)(feas[126].get());
	for (size_t v = 0; v < feas.size(); v++)
	{
		HeatKernelSignatureDesc* hksVV = (HeatKernelSignatureDesc*)(feas[v].get());
		const double l2Distance = HeatKernelSignatureDesc::L2Distance(*hksVV, *hksV0);
		distances.push_back(l2Distance);
	}

	const double maxDistance = *std::max_element(distances.begin(), distances.end());
	const double minDistance = *std::min_element(distances.begin(), distances.end());

	for (size_t i = 0; i < distances.size(); i++)
	{
		distances[i] = (distances[i] - minDistance) / (maxDistance - minDistance);
	}

	mesh.showVertexColors(distances, 126);
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
	/*return HKSExtractionTestAPP(argc, argv);*/
	/*return OnEdgeAvgDistExtractionTestApp(argc, argv);*/
	return GlobalDescForLocalPatchesTestApp(argc, argv);
}
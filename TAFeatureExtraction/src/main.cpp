#include <iostream>
#include <core/TAMatrix.h>
#include <core/TriangularMesh.h>
#include <core/CommandLineParser.h>
#include <core/Timer.h>
#include <core/StdVectorUtil.h>
#include <core/StringUtil.h>
#include <core/PathUtil.h>

#include <oi/ThreeDimOIShape.h>

//Descriptors
#include "AvgGeodesicDistance.h"
#include "IntrinsicWaveDesc.h"
#include "HeatKernelSignatureDesc.h"
#include "PatchBasedSumOfCenterDistances.h"
#include "PatchBasedShapeDistributionHistogram.h"

//Extractors
#include "OnEdgeAvgGeoDistExtraction.h"
#include "IntrinsicWaveDescExtraction.h"
#include "GeodesicDistanceMatrixExtraction.h"
#include "PatchBasedPerVertexFeatureExtraction.h"
#include "PatchBasedSumOfCenterDistancesExtraction.h"
#include "PatchBasedShapeDistributionDescExtraction.h"
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

int GlobalDescForLocalPatchesTestSumOfCenterDistancesApp(int argc, char* argv[])
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

	TAFeaExt::PatchBasedSumOfCenterDistancesExtraction patchBasedExtractor;
	patchBasedExtractor.setMinGeodesicDistance(20.0f);
	patchBasedExtractor.setMaxGeodesicDistance(100.0f);
	patchBasedExtractor.setNumberOfPatches(20);

	std::vector<LocalFeaturePtr> feas;
	patchBasedExtractor.extract(triMesh, feas);

	std::vector<double> magnitudes(feas.size());
	for (size_t v = 0; v < feas.size(); v++)
	{
		PatchBasedSumOfCenterDistances* feaVV = (PatchBasedSumOfCenterDistances*)(feas[v].get());
		magnitudes[v] = StdVecL2Norm(feaVV->m_vDescriptor);
	}

	const double maxMag = *std::max_element(magnitudes.begin(), magnitudes.end());
	const double minMag = *std::min_element(magnitudes.begin(), magnitudes.end());

	for (size_t i = 0; i < magnitudes.size(); i++)
	{
		magnitudes[i] = (magnitudes[i] - minMag) / (maxMag - minMag);
	}

	mesh.showVertexColors(magnitudes);
	return mainRet(1, "Main Test Successfully Ended");
}

int GlobalDescForLocalPatchesTestShapeDistributionApp(int argc, char* argv[])
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

	ThreeDimOIShape mesh;
	if (mesh.load(inputFile.c_str()) != TACORE_OK)
	{
		return mainRet(1, "Mesh cannot be loaded correctly");
	}

	TriangularMesh* triMesh = (TriangularMesh*)(mesh.getShape());

	TAFeaExt::PatchBasedShapeDistributionDescExtraction patchBasedExtractor;
	patchBasedExtractor.setMinGeodesicDistance(20.f);
	patchBasedExtractor.setMaxGeodesicDistance(300);
	patchBasedExtractor.setNumberOfPatches(30);
	patchBasedExtractor.setNumberOfBins(8);
	patchBasedExtractor.setSampleCount(1024);
	patchBasedExtractor.setSamplingMethod(TAFeaExt::PatchBasedShapeDistributionDescExtraction::BIASED_VERTEX_SAMPLING);
	patchBasedExtractor.setShapeDistributionFunction(TAFeaExt::PatchBasedShapeDistributionDescExtraction::DISTANCE_BETWEEN_FIXED_AND_RANDOM_POINT);

	std::vector<LocalFeaturePtr> feas;
	patchBasedExtractor.extract(triMesh, feas);

	const int referenceVertexId = 126;
	PatchBasedShapeDistributionHistogram* feaV_Src = (PatchBasedShapeDistributionHistogram*)(feas[referenceVertexId].get());

	std::vector<double> magnitudes(feas.size());
	for (size_t v = 0; v < feas.size(); v++)
	{
		PatchBasedShapeDistributionHistogram* feaVV = (PatchBasedShapeDistributionHistogram*)(feas[v].get());
		std::vector<double> vertexPatchMagnitudes(feaVV->m_vDescriptor.size());
		for (size_t patchIdx = 0; patchIdx < vertexPatchMagnitudes.size(); patchIdx++)
		{
			vertexPatchMagnitudes[patchIdx] = StdVecL2Norm(feaVV->m_vDescriptor[patchIdx] - feaV_Src->m_vDescriptor[patchIdx]);
		}
		magnitudes[v] = StdVecL2Norm(vertexPatchMagnitudes);
	}

	const double maxMag = *std::max_element(magnitudes.begin(), magnitudes.end());
	const double minMag = *std::min_element(magnitudes.begin(), magnitudes.end());

	for (size_t i = 0; i < magnitudes.size(); i++)
	{
		magnitudes[i] = (magnitudes[i] - minMag) / (maxMag - minMag);
	}

	mesh.showVertexColors(magnitudes, referenceVertexId);

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
	for (size_t v = 0; v < feas.size(); v++)
	{
		AvgGeodesicDistance* avgGeoDistFeaPtr = (AvgGeodesicDistance*)(feas[v].get());
		distances[v] = avgGeoDistFeaPtr->m_distance;
	}

	const double maxDistance = *std::max_element(distances.begin(), distances.end());
	const double minDistance = *std::min_element(distances.begin(), distances.end());

	for (size_t i = 0; i < distances.size(); i++)
	{
		distances[i] = (distances[i] - minDistance) / (maxDistance - minDistance);
	}

	mesh.showVertexColors(distances);
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
	hksExtractor.setTypeOfLaplacian(TAFeaExt::HKSDescExtraction::STAR_LAPLACIAN);

	std::vector<LocalFeaturePtr> feas;
	hksExtractor.extract(triMesh, feas);

	HeatKernelSignatureDesc* hksV_Src = (HeatKernelSignatureDesc*)(feas[126].get());

	std::vector<double> magnitudes(feas.size());
	for (size_t v = 0; v < feas.size(); v++)
	{
		HeatKernelSignatureDesc* hksVV = (HeatKernelSignatureDesc*)(feas[v].get());
		magnitudes[v] = StdVecL2Norm(hksVV->m_vDescriptor - hksV_Src->m_vDescriptor);
	}

	const double maxMag = *std::max_element(magnitudes.begin(), magnitudes.end());
	const double minMag = *std::min_element(magnitudes.begin(), magnitudes.end());

	for (size_t i = 0; i < magnitudes.size(); i++)
	{
		magnitudes[i] = (magnitudes[i] - minMag) / (maxMag - minMag);
	}

	mesh.showVertexColors(magnitudes, 126);
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
	/*return GlobalDescForLocalPatchesTestSumOfCenterDistancesApp(argc, argv);*/
	return GlobalDescForLocalPatchesTestShapeDistributionApp(argc, argv);
}
#include "MultiParameterPerVertexDescriptorRun/MultiParameterFeatureComparisonMain.h"
#include "MultiParameterPerVertexDescriptorRun/PBSDParameterSet.h"
#include <core/CommandLineParser.h>
#include <core/TriangularMesh.h>
#include <core/StringUtil.h>
#include <core/PathUtil.h>
#include <core/StdVectorUtil.h>
#include <TAFeatureFileIO.h>
#include <fstream>

//Descriptors
#include "PatchBasedShapeDistributionHistogram.h"
#include "AvgGeodesicDistance.h"

//Extractors
#include "OnEdgeAvgGeoDistExtraction.h"

MultiParameterFeatureComparisonMain::Result MultiParameterFeatureComparisonMain::run(int argc, char* argv[])
{
	Result res = MPFCR_OK;

	// Parse args
	TACore::ArgParser parser("MultiParameterFeatureComparisonMain::Run", "Generates a report about comparsion of TWO types of per vertex features");
	parser.addArg("input-mesh", "", true, 1, "", "Input shape file to be loaded");
	parser.addArg("src-desc-type", "", true, 1, "", "Type of the source descriptor read");
	parser.addArg("ref-desc-type", "", true, 1, "", "Type of the reference descriptor extracted");
	parser.addArg("input-src-fea-folder", "", true, 1, "", "Input folder for source features");
	parser.addArg("ref-vertex", "", true, 1, "", "Reference vertex which is used to calculate comparison metrics");
	parser.addArg("out-report-path", "", true, 1, "", "Path to output report file");

	if (!parser.parseCommandLine(argc, argv))
	{
		res = MPFCR_COMMAND_LINE_PARSE_ERROR;
	}

	if (res == MPFCR_OK)
	{
		std::string inputMeshFile = parser.get("input-mesh");
		std::string srcDescType = parser.get("src-desc-type");
		std::string refDescType = parser.get("ref-desc-type");
		std::string inpSrcFeaFolder = parser.get("input-src-fea-folder");
		int refVertex = parser.getInt("ref-vertex");
		std::string outReportPath = parser.get("out-report-path");

		//Read the mesh
		TAShape::TriangularMesh triMesh;
		if (triMesh.load(inputMeshFile.c_str()) != TACore::TACORE_OK)
		{
			res = MPFCR_MESH_LOAD_ERROR;
		}

		if (res == MPFCR_OK)
		{
			std::vector<std::string> featureFilePaths;
			TACore::PathUtil::getDirectoryEntries(inpSrcFeaFolder, featureFilePaths, "fea", false, false);

			//Decide which source descriptor is read
			bool readPBSDSourcePermutations = false;
			if (srcDescType == "pbsd")
			{
				readPBSDSourcePermutations = true;
			}

			//Decide which reference descriptor is extracted
			bool extractAGDReference = false;
			if (refDescType == "agd")
			{
				extractAGDReference = true;
			}

			//Comparison values of the source features from all permutation of parameters
			std::map<int, std::vector<double> > srcPermutationMagnitudes;
			if (readPBSDSourcePermutations == true)
			{
				srcPermutationMagnitudes = createPermutationIdToMagnitudesMapForPBSD(featureFilePaths, refVertex);
			}

			std::vector<double> refMagnitudes;
			if (extractAGDReference == true)
			{
				refMagnitudes = createAGDMagnitudes(&triMesh, refVertex);
			}

			res = generateComparisonResult(refMagnitudes, srcPermutationMagnitudes, outReportPath);
		}
	}
	return res;
}

std::map<int, std::vector<double> > MultiParameterFeatureComparisonMain::createPermutationIdToMagnitudesMapForPBSD(const std::vector<std::string>& featureFilePaths, const int& refVertex)
{
	std::map<int, std::vector<double> > resultMap;
	for (size_t i = 0; i < featureFilePaths.size(); i++)
	{
		if (i < featureFilePaths.size() - 1)
		{
			std::cout << "%" << (100 * i) / featureFilePaths.size() << " completed for parsing the feature files" << "\r";
		}
		else
		{
			std::cout << "%" << 100 << " completed for parsing the feature files" << "\n";
		}

		const std::string filePath = featureFilePaths[i];
		const std::string fileName = TACore::PathUtil::getFileNameFromPath(filePath);

		std::vector<std::string> splitValues = TACore::StringUtil::split(fileName, '_');

		const int permutationId = TACore::StringUtil::strToVal<int>(splitValues[0]); //permutationId must be in the 0th index.

		std::vector<TAFea::LocalFeaturePtr> fileFeatures;
		TAFea::TAFeatureFileIO::readPBSDInBinary(filePath, fileFeatures);

		TAFea::PatchBasedShapeDistributionHistogram* feaV_Ref = (TAFea::PatchBasedShapeDistributionHistogram*)(fileFeatures[refVertex].get());
		std::vector<double> magnitudes(fileFeatures.size());
		for (size_t v = 0; v < fileFeatures.size(); v++)
		{
			TAFea::PatchBasedShapeDistributionHistogram* feaVV = (TAFea::PatchBasedShapeDistributionHistogram*)(fileFeatures[v].get());
			std::vector<double> vertexPatchMagnitudes(feaVV->m_vDescriptor.size());
			for (size_t patchIdx = 0; patchIdx < vertexPatchMagnitudes.size(); patchIdx++)
			{
				vertexPatchMagnitudes[patchIdx] = StdVecL2Norm(feaVV->m_vDescriptor[patchIdx] - feaV_Ref->m_vDescriptor[patchIdx]);
			}
			magnitudes[v] = StdVecL2Norm(vertexPatchMagnitudes);
		}

		const double maxMag = *std::max_element(magnitudes.begin(), magnitudes.end());
		const double minMag = *std::min_element(magnitudes.begin(), magnitudes.end());

		for (size_t i = 0; i < magnitudes.size(); i++)
		{
			magnitudes[i] = (magnitudes[i] - minMag) / (maxMag - minMag);
		}

		resultMap[permutationId] = magnitudes;
	}

	return resultMap;
}

std::vector<double> MultiParameterFeatureComparisonMain::createAGDMagnitudes(TAShape::TriangularMesh* triMesh, const int& refVertex)
{
	TAFeaExt::OnEdgeAvgGeoDistExtraction avgGeoExtractor;
	std::vector<LocalFeaturePtr> feas;
	avgGeoExtractor.extract(triMesh, feas);

	std::vector<double> magnitudes(feas.size());
	AvgGeodesicDistance* avgGeoDistFeaPtrRefVertex = (AvgGeodesicDistance*)(feas[refVertex].get());
	for (size_t v = 0; v < feas.size(); v++)
	{
		AvgGeodesicDistance* avgGeoDistFeaPtr = (AvgGeodesicDistance*)(feas[v].get());
		magnitudes[v] = sqrt((avgGeoDistFeaPtr->m_distance - avgGeoDistFeaPtrRefVertex->m_distance) * 
							(avgGeoDistFeaPtr->m_distance - avgGeoDistFeaPtrRefVertex->m_distance));
	}

	const double maxDistance = *std::max_element(magnitudes.begin(), magnitudes.end());
	const double minDistance = *std::min_element(magnitudes.begin(), magnitudes.end());

	for (size_t i = 0; i < magnitudes.size(); i++)
	{
		magnitudes[i] = (magnitudes[i] - minDistance) / (maxDistance - minDistance);
	}

	return magnitudes;
}

MultiParameterFeatureComparisonMain::Result MultiParameterFeatureComparisonMain::generateComparisonResult(const std::vector<double>& referenceMagnitudes, const std::map<int, std::vector<double> >& permutationIdToSourceMagnitudesMap, const std::string& outFileName)
{
	Result res = MPFCR_OK;

	std::ofstream out(outFileName, std::ios::out);
	if (out.is_open())
	{
		std::map<double, int> scoreMap;
		std::map<int, std::vector<double> >::const_iterator itr = permutationIdToSourceMagnitudesMap.begin();
		for (; itr != permutationIdToSourceMagnitudesMap.end(); itr++)
		{
			const double scoreOfPermutation = StdVecL2Distance(itr->second, referenceMagnitudes);
			scoreMap[scoreOfPermutation] = itr->first;
		}

		std::map<double, int>::const_iterator scitr = scoreMap.begin();
		for (; scitr != scoreMap.end(); scitr++)
		{
			out << scitr->first << " --> " << scitr->second << std::endl;
		}

		std::cout << "Report Generation is Completed..." << std::endl;

		out.close();
	}
	else
	{
		res = MPFCR_FILE_ERROR;
	}
	return res;
}
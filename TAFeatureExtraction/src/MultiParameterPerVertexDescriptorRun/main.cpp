#include <iostream>
#include <core/CommandLineParser.h>
#include <core/Defs.h>
#include <core/TriangularMesh.h>
#include <core/StdVectorUtil.h>
#include <core/StringUtil.h>
#include <core/PathUtil.h>
#include <TAFeatureFileIO.h>
#include <MultiParameterPerVertexDescriptorRun/PBSDParameterSet.h>

//Descriptors
#include "PatchBasedShapeDistributionHistogram.h"

//Extractors
#include "PatchBasedShapeDistributionDescExtraction.h"

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
	TACore::ArgParser parser("MultiParameterPerVertexDescriptionRun", "Runs feature extraction tool for a specified feature for a set of parametes");
	parser.addArg("input-mesh", "", true, 1, "", "Input shape file to be loaded");
	parser.addArg("desc-type", "", true, 1, "", "Type of the descriptor extracted");
	parser.addArg("ref-vertex", "", false, 1, "", "Reference vertex which is compared to other vertices"); //If this is given, a color ply file is also created as output
	parser.addArg("out-folder", "", true, 1, "", "The folder in which the output files are created");

	if (!parser.parseCommandLine(argc, argv))
	{
		mainRet(-1, "Command line parameters cannot be parsed correctly");
	}

	std::string inputMeshFile = parser.get("input-mesh");
	std::string descType = parser.get("desc-type");
	int refVertex = -1;
	if (parser.exists("ref-vertex"))
	{
		refVertex = parser.getInt("ref-vertex");
	}
	std::string outFolder = parser.get("out-folder");

	//Decide whether or not to create ply outputs
	const bool createComparisonPly = (refVertex != -1);

	//Read the mesh
	TriangularMesh triMesh;
	if (triMesh.load(inputMeshFile.c_str()) != TACORE_OK)
	{
		return mainRet(1, "Mesh cannot be loaded correctly");
	}

	//Decide which descriptor is run
	bool runPBSDPermutations = false;

	if (descType == "pbsd")
	{
		runPBSDPermutations = true;
	}

	if (runPBSDPermutations == true)
	{
		PBSDParameterSet minValues(20.0f,	50.0f,	10,	8,	128,	1,	1);
		PBSDParameterSet maxValues(21.0f,	301.0f, 50,	32,	1024,	2,	4);
		PBSDParameterSet incrValues(10.0f,	50.0f,	20,	8,	128,	10,	1);
		std::vector<PBSDParameterSet> permutations = PBSDParameterSet::createPermutations(minValues, maxValues, incrValues);

		std::cout << "Number of Permutations: " << permutations.size() << std::endl;
		std::cout << "Reference Vertex Id is " << refVertex << std::endl;
		for (size_t permId = 0; permId < permutations.size(); permId++)
		{
			const PBSDParameterSet& permutation = permutations[permId];

			TAFeaExt::PatchBasedShapeDistributionDescExtraction patchBasedExtractor;
			patchBasedExtractor.setMinGeodesicDistance(permutation.m_fMinGeodesicDistance);
			patchBasedExtractor.setMaxGeodesicDistance(permutation.m_fMaxGeodesicDistance);
			patchBasedExtractor.setNumberOfPatches(permutation.m_nNumberOfPatches);
			patchBasedExtractor.setNumberOfBins(permutation.m_nNumberOfBins);
			patchBasedExtractor.setSampleCount(permutation.m_nSampleCount);
			patchBasedExtractor.setSamplingMethod(TAFeaExt::PatchBasedShapeDistributionDescExtraction::SamplingMethod(permutation.m_nSamplingMethod));
			patchBasedExtractor.setShapeDistributionFunction(TAFeaExt::PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction(permutation.m_nShapeDistributionFunction));

			std::vector<LocalFeaturePtr> feas;
			patchBasedExtractor.extract(&triMesh, feas);

			//Get the name of permutation. This will be used as the base names for the output files
			const std::string permutationName = permutation.getName(TACore::PathUtil::getFileNameFromPath(inputMeshFile));

			//Write the binary file of features
			const std::string featureFilePath = TACore::PathUtil::joinPath(outFolder, TACore::PathUtil::addExtension(permutationName, "fea"));
			TAFea::TAFeatureFileIO::writePBSDInBinary(feas, featureFilePath);

			//Write ply file with color according to a reference vertex
			if (createComparisonPly == true)
			{
				PatchBasedShapeDistributionHistogram* feaV_Ref = (PatchBasedShapeDistributionHistogram*)(feas[refVertex].get());

				std::vector<double> magnitudes(feas.size());
				for (size_t v = 0; v < feas.size(); v++)
				{
					PatchBasedShapeDistributionHistogram* feaVV = (PatchBasedShapeDistributionHistogram*)(feas[v].get());
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
				
				const std::string plyFilePath = TACore::PathUtil::joinPath(outFolder, TACore::PathUtil::addExtension(permutationName, "ply"));
				triMesh.save(plyFilePath.c_str(), magnitudes);
			}		
		}
	}

	return mainRet(1, "Main Test Successfully Ended");
}
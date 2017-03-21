#include "MultiParameterPerVertexDescriptorRun/MultiParameterFeatureExtractionMain.h"
#include "MultiParameterPerVertexDescriptorRun/PBSDParameterSet.h"
#include <core/CommandLineParser.h>
#include <core/TriangularMesh.h>
#include <core/TriMeshAuxInfo.h>
#include <core/StdVectorUtil.h>
#include <core/StringUtil.h>
#include <core/PathUtil.h>
#include <TAFeatureFileIO.h>

//Descriptors
#include "GeodesicDistanceMatrix.h"
#include "PatchBasedShapeDistributionHistogram.h"

//Extractors
#include "PatchBasedShapeDistributionDescExtraction.h"

MultiParameterFeatureExtractionMain::Result MultiParameterFeatureExtractionMain::run(int argc, char* argv[])
{
	Result res = MPFER_OK;

	// Parse args
	TACore::ArgParser parser("MultiParameterFeatureExtractionMain::Run", "Runs feature extraction tool for a specified feature for a set of parametes");
	parser.addArg("input-mesh", "", true, 1, "", "Input shape file to be loaded");
	parser.addArg("desc-type", "", true, 1, "", "Type of the descriptor extracted");
	parser.addArg("gd-matrix-path", "", true, 1, "", "Geodesic distance matrix of the mesh for efficiency");
	parser.addArg("aux-info-path", "", true, 1, "", "Auxilary information about the mesh for efficiency");
	parser.addArg("out-fea-folder", "", true, 1, "", "The folder in which the output feature files are created");
	parser.addArg("ref-vertex", "", false, 1, "", "Reference vertex which is compared to other vertices"); //If this is given, a color ply file is also created as output
	parser.addArg("out-ply-folder", "", false, 1, "", "The folder in which the output ply files are created");

	if (!parser.parseCommandLine(argc, argv))
	{
		res = MPFER_COMMAND_LINE_PARSE_ERROR;
	}

	if (res == MPFER_OK)
	{
		std::string inputMeshFile = parser.get("input-mesh");
		std::string descType = parser.get("desc-type");
		std::string gdMatrixPath = parser.get("gd-matrix-path");
		std::string auxInfoPath = parser.get("aux-info-path");
		std::string outFeaFolder = parser.get("out-fea-folder");
		int refVertex = -1;
		if (parser.exists("ref-vertex"))
		{
			refVertex = parser.getInt("ref-vertex");
		}
		std::string outPlyFolder = "";
		if (parser.exists("out-ply-folder"))
		{
			outPlyFolder = parser.get("out-ply-folder");
		}	

		//Decide whether or not to create ply outputs
		const bool createComparisonPly = (refVertex != -1) && (outPlyFolder != "");

		//Read the mesh
		TAShape::TriangularMesh triMesh;
		if (triMesh.load(inputMeshFile.c_str()) != TACore::TACORE_OK)
		{
			res = MPFER_MESH_LOAD_ERROR;
		}

		if (res == MPFER_OK)
		{
			//Decide which descriptor is run
			bool runPBSDPermutations = false;

			if (descType == "pbsd")
			{
				runPBSDPermutations = true;
			}

			if (runPBSDPermutations == true)
			{
				//Read Geodesic Distance Matrix
				TAFea::GeodesicDistanceMatrix gdMatrix;
				gdMatrix.m_GeoMatrix.loadBinary(gdMatrixPath);

				//Read Auxilary info about the mesh
				TAShape::TriMeshAuxInfo auxInfo;
				auxInfo.load(auxInfoPath);

				PBSDParameterSet minValues(20.0f, 50.0f, 10, 8, 128, 1, 1);
				PBSDParameterSet maxValues(21.0f, 301.0f, 50, 32, 1024, 2, 5);
				PBSDParameterSet incrValues(10.0f, 50.0f, 20, 8, 128, 10, 1);
				std::vector<PBSDParameterSet> permutations = PBSDParameterSet::createPermutations(minValues, maxValues, incrValues);

				std::cout << "Number of Permutations: " << permutations.size() << std::endl;
				std::cout << "Reference Vertex Id is " << refVertex << std::endl;
				for (size_t permId = 0; permId < permutations.size(); permId++)
				{
					const PBSDParameterSet& permutation = permutations[permId];

					TAFeaExt::PatchBasedShapeDistributionDescExtraction patchBasedExtractor;
					patchBasedExtractor.setGeodesicDistanceMatrix(&gdMatrix);
					patchBasedExtractor.setMinGeodesicDistance(permutation.m_fMinGeodesicDistance);
					patchBasedExtractor.setMaxGeodesicDistance(permutation.m_fMaxGeodesicDistance);
					patchBasedExtractor.setNumberOfPatches(permutation.m_nNumberOfPatches);
					patchBasedExtractor.setNumberOfBins(permutation.m_nNumberOfBins);
					patchBasedExtractor.setSampleCount(permutation.m_nSampleCount);
					patchBasedExtractor.setSamplingMethod(TAFeaExt::PatchBasedShapeDistributionDescExtraction::SamplingMethod(permutation.m_nSamplingMethod));
					patchBasedExtractor.setShapeDistributionFunction(TAFeaExt::PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction(permutation.m_nShapeDistributionFunction));
					if (TAFeaExt::PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction(permutation.m_nShapeDistributionFunction)
						== TAFeaExt::PatchBasedShapeDistributionDescExtraction::DISTANCE_BETWEEN_TWO_RANDOM_POINTS)
					{
						patchBasedExtractor.setMaxPossibleSampleValue(auxInfo.m_lfMaxEucDistanceBetweenTwoVertices);
					}
					else if (TAFeaExt::PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction(permutation.m_nShapeDistributionFunction)
						== TAFeaExt::PatchBasedShapeDistributionDescExtraction::SQRT_OF_AREA_OF_THREE_RANDOM_POINTS)
					{
						patchBasedExtractor.setMaxPossibleSampleValue(sqrt(auxInfo.m_lfMaxAreaOfTriangleConstructedByThreeVertices));
					}
					else if (TAFeaExt::PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction(permutation.m_nShapeDistributionFunction)
						== TAFeaExt::PatchBasedShapeDistributionDescExtraction::CBRT_OF_VOLUME_OF_FOUR_RANDOM_POINTS)
					{
						patchBasedExtractor.setMaxPossibleSampleValue(cbrt(auxInfo.m_lfMaxVolumeOfTetrahedronConstructedByForVertices));
					}

					std::vector<LocalFeaturePtr> feas;
					patchBasedExtractor.extract(&triMesh, feas);

					//Get the name of permutation. This will be used as the base names for the output files
					const std::string permutationName = permutation.getName(TACore::PathUtil::getFileNameFromPath(inputMeshFile), permId);

					//Write the binary file of features
					const std::string featureFilePath = TACore::PathUtil::joinPath(outFeaFolder, TACore::PathUtil::addExtension(permutationName, "fea"));
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

						const std::string plyFilePath = TACore::PathUtil::joinPath(outPlyFolder, TACore::PathUtil::addExtension(permutationName, "ply"));
						triMesh.save(plyFilePath.c_str(), magnitudes);
					}
				}
			}
		}
	}
	return res;
}
#ifndef PATCH_BASED_SUM_OF_CENTER_DISTANCES_EXTRACTION_H
#define PATCH_BASED_SUM_OF_CENTER_DISTANCES_EXTRACTION_H

#include "PatchBasedPerVertexFeatureExtraction.h"

namespace TAFeaExt
{
	//PatchBasedSumOfCenterDistancesExtraction is a PatchBasedPerVertexFeatureExtraction
	//See PatchBasedPerVertexFeatureExtraction.h to answer the questions like what is a patch
	//This extraction tool extracts PatchBasedSumOfCenterDistances descriptor for a vertex or all vertices
	//See PatchBasedSumOfCenterDistances.h to get the information about the descriptor
	//No extract methods. The objects of this class use the extract function of PatchBasedPerVertexFeatureExtraction
	class PatchBasedSumOfCenterDistancesExtraction : public PatchBasedPerVertexFeatureExtraction
	{
	public:

		virtual ~PatchBasedSumOfCenterDistancesExtraction();
		PatchBasedSumOfCenterDistancesExtraction();
		PatchBasedSumOfCenterDistancesExtraction(const PatchBasedSumOfCenterDistancesExtraction& other);

		/**
		* @brief	Gets the type of the global descriptor extracted from each patch
		* @return	SUM_EUC_DISTANCES_TO_CENTER
		*/
		virtual TypeOfGlobalDescriptor getGlobalDescriptorType() const;

	protected:

		/**
		* @brief	Calculates patch based sum of center distances from extracted patches for ALL VERTICES
		*
		* @param	triMesh TriangularMesh
		* @param	listOfPatchLists patches of all vertices
		* @param	[out] outFeatures Output vector of features
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if triMesh is NULL
		* @return	TACORE_BAD_ARGS if listOfPatchLists is empty
		*/
		virtual Result calcFeature(TriangularMesh* triMesh, const std::vector<PatchList>& listOfPatchLists, std::vector<LocalFeaturePtr>& outFeatures);

		/**
		* @brief	Calculates patch based sum of center distances from extracted patches for a vertex
		*
		* @param	triMesh TriangularMesh
		* @param	id Id of the vertex whose feature is being extracted
		* @param	patchesVertexIds patches of a vertex
		* @param	[out] outFeaturePtr Output feature pointer
		* @return	TACORE_OK if everything goes fine
		*/
		virtual Result calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr);
	};
}

#endif
#include "PatchBasedSumOfCenterDistancesExtraction.h"
#include "PatchBasedSumOfCenterDistances.h"
#include <core/TriangularMesh.h>
#include <core/NDimVector.h>

namespace TAFeaExt
{
	PatchBasedSumOfCenterDistancesExtraction::~PatchBasedSumOfCenterDistancesExtraction()
	{

	}

	PatchBasedSumOfCenterDistancesExtraction::PatchBasedSumOfCenterDistancesExtraction()
	{

	}

	PatchBasedSumOfCenterDistancesExtraction::PatchBasedSumOfCenterDistancesExtraction(const PatchBasedSumOfCenterDistancesExtraction& other)
	{

	}

	PatchBasedPerVertexFeatureExtraction::TypeOfGlobalDescriptor PatchBasedSumOfCenterDistancesExtraction::getGlobalDescriptorType() const
	{
		return PatchBasedPerVertexFeatureExtraction::SUM_EUC_DISTANCES_TO_CENTER;
	}

	Result PatchBasedSumOfCenterDistancesExtraction::calcFeature(TriangularMesh* triMesh, const std::vector<PatchList>& listOfPatchLists, std::vector<LocalFeaturePtr>& outFeatures)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);

		const size_t verSize = triMesh->verts.size();
		outFeatures = std::vector<LocalFeaturePtr>(verSize);

		for (size_t v = 0; v < verSize; v++)
		{
			if (v < verSize - 1)
			{
				std::cout << "%" << (100 * v) / verSize << " completed for creating descriptors from patches for all vertices" << "\r";
			}
			else
			{
				std::cout << "%" << 100 << " completed for creating descriptors from patches for all vertices" << "\n";
			}

			calcFeature(triMesh, v, listOfPatchLists[v], outFeatures[v]);
		}

		return TACore::TACORE_OK;
	}

	Result PatchBasedSumOfCenterDistancesExtraction::calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr)
	{
		const size_t numberOfPatchesV = patchesVertexIds.size();
		PatchBasedSumOfCenterDistances* pDesc = new PatchBasedSumOfCenterDistances(id);
		pDesc->m_vDescriptor = std::vector<double>(numberOfPatchesV);

		for (size_t patchIdx = 0; patchIdx < numberOfPatchesV; patchIdx++)
		{
			const SinglePatch& aPatch = patchesVertexIds[patchIdx];
			Vector3D patchCenter(0.0, 0.0, 0.0);

			for (size_t w = 0; w < aPatch.size(); w++)
			{
				Vertex* patchVertex = triMesh->verts[aPatch[w]];
				patchCenter = patchCenter + Vector3D(patchVertex->coords[0], patchVertex->coords[1], patchVertex->coords[2]);
			}
			patchCenter = patchCenter * (1.0 / ((double)aPatch.size()));

			double sumOfCenterDistances = 0.0;
			for (size_t w = 0; w < aPatch.size(); w++)
			{
				Vertex* patchVertex = triMesh->verts[aPatch[w]];
				sumOfCenterDistances += Vector3D::L2Distance(patchCenter, Vector3D(patchVertex->coords[0], patchVertex->coords[1], patchVertex->coords[2]));
			}

			pDesc->m_vDescriptor[patchIdx] = sumOfCenterDistances;
		}

		outFeaturePtr = LocalFeaturePtr(pDesc);

		return TACore::TACORE_OK;
	}
}
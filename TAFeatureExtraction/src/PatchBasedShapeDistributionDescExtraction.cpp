#include "PatchBasedShapeDistributionDescExtraction.h"
#include "PatchBasedShapeDistributionHistogram.h"
#include <core/TriangularMesh.h>
#include <core/NDimVector.h>
#include <core/StdVectorUtil.h>
#include <unordered_set>

namespace TAFeaExt
{
	PatchBasedShapeDistributionDescExtraction::~PatchBasedShapeDistributionDescExtraction()
	{
		
	}

	PatchBasedShapeDistributionDescExtraction::PatchBasedShapeDistributionDescExtraction()
	{
		this->m_DistributionFunction = ANGLE_BETWEEN_THREE_RANDOM_POINTS;
		this->m_SamplingMethod = BIASED_VERTEX_SAMPLING;
		this->m_fSampleCountDecrementRatio = 1.0f;
	}

	PatchBasedShapeDistributionDescExtraction::PatchBasedShapeDistributionDescExtraction(const PatchBasedShapeDistributionDescExtraction& other)
	{
		this->m_DistributionFunction = other.m_DistributionFunction;
		this->m_SamplingMethod = other.m_SamplingMethod;
		this->m_fSampleCountDecrementRatio = other.m_fSampleCountDecrementRatio;
	}

	void PatchBasedShapeDistributionDescExtraction::setShapeDistributionFunction(const ShapeDistributionFunction& shapeDistributionFunction)
	{
		this->m_DistributionFunction = shapeDistributionFunction;
	}

	PatchBasedShapeDistributionDescExtraction::ShapeDistributionFunction 
	PatchBasedShapeDistributionDescExtraction::getShapeDistributionFunction() const
	{
		return this->m_DistributionFunction;
	}

	void PatchBasedShapeDistributionDescExtraction::setSamplingMethod(const SamplingMethod& samplingMethod)
	{
		this->m_SamplingMethod = samplingMethod;
	}

	PatchBasedShapeDistributionDescExtraction::SamplingMethod 
	PatchBasedShapeDistributionDescExtraction::getSamplingMethod() const
	{
		return this->m_SamplingMethod;
	}

	void PatchBasedShapeDistributionDescExtraction::setSampleCountDecrementRatio(const float& sampleCountDecrementRatio)
	{
		this->m_fSampleCountDecrementRatio = sampleCountDecrementRatio;
	}

	float PatchBasedShapeDistributionDescExtraction::getSampleCountDecrementRatio() const
	{
		return this->m_fSampleCountDecrementRatio;
	}

	PatchBasedPerVertexFeatureExtraction::TypeOfGlobalDescriptor PatchBasedShapeDistributionDescExtraction::getGlobalDescriptorType() const
	{
		return PatchBasedPerVertexFeatureExtraction::SHAPE_DISTRIBUTION_HISTOGRAM;
	}

	Result PatchBasedShapeDistributionDescExtraction::calcFeature(TriangularMesh* triMesh, const std::vector<PatchList>& listOfPatchLists, std::vector<LocalFeaturePtr>& outFeatures)
	{
		TACORE_CHECK_ARGS(triMesh != NULL);
		TACORE_CHECK_ARGS(listOfPatchLists.size() > 0);

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

	Result PatchBasedShapeDistributionDescExtraction::calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr)
	{
		const size_t numberOfPatchesV = patchesVertexIds.size();
		PatchBasedShapeDistributionHistogram *pDesc = new PatchBasedShapeDistributionHistogram(id);

		//A vertex will contain a vector for each patch defining the probability distribution function of the samples
		//Each sample will be counted inside the corresponding bins
		//Therefore descriptor has the size of numberOfPatches * numberOfBins
		pDesc->m_vDescriptor = std::vector<std::vector<double> >(numberOfPatchesV, std::vector<double>(this->m_nNumberOfBins));

		for (size_t patchIdx = 0; patchIdx < numberOfPatchesV; patchIdx++)
		{
			const SinglePatch& aPatch = patchesVertexIds[patchIdx];

			//Number of samples depends on the patch size
			//This will be different for different patchs and for different meshes
			//Normalization will be required in order to compare pathches correctly: Histogram ---> PDF
			const int sampleCount = int(float(aPatch.size() * (aPatch.size() - 1)) * 0.5f * this->m_fSampleCountDecrementRatio);

			

			getchar();
		}

		return TACore::TACORE_OK;
	}

	void PatchBasedShapeDistributionDescExtraction::createSamplesFromPatch(TriangularMesh* triMesh, const SinglePatch& patch, const int& sampleCount, std::vector<double>& samples) const
	{
		samples = std::vector<double>(sampleCount);
		for (int i = 0; i < sampleCount; i++)
		{
			samples[i] = createSample(triMesh, patch);
		}

		//TO DO:
		//Normalization and etc.
	}

	double PatchBasedShapeDistributionDescExtraction::createSample(TriangularMesh* triMesh, const SinglePatch& patch) const
	{
		if (this->m_SamplingMethod == BIASED_VERTEX_SAMPLING)
		{
			return createBiasedVertexSample(triMesh, patch);
		}
		return createUnbiasedSurfaceSample(triMesh, patch);
	}

	double PatchBasedShapeDistributionDescExtraction::createBiasedVertexSample(TriangularMesh* triMesh, const SinglePatch& patch) const
	{
		size_t numberOfRandomPoints = getNumberOfRandomPointsForASample();
		std::unordered_set<Vertex*> randomVertices; //Use pointer equality to select different random vertices
													//Use unordered_set instead of set in order not to integrate much bias because of the pointer locations
		while (randomVertices.size() < numberOfRandomPoints)
		{
			int randomVertexId = rand() % patch.size();
			randomVertices.insert(triMesh->verts[randomVertexId]);
		}

		std::unordered_set<Vertex*>::iterator ptItr = randomVertices.begin();

		double ret = 0.0;
		if (this->m_DistributionFunction == ANGLE_BETWEEN_THREE_RANDOM_POINTS)
		{
			Vector3D pt1((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt2((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt3((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]);

			ret = calcAngleBetweenThreePoints(pt1, pt2, pt2);
		}
		else if (this->m_DistributionFunction == DISTANCE_BETWEEN_FIXED_AND_RANDOM_POINT)
		{
			//The index of a fixed point is always 0
			Vector3D pt1(triMesh->verts[0]->coords[0], triMesh->verts[0]->coords[1], triMesh->verts[0]->coords[2]);
			Vector3D pt2((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]);

			ret = calcDistanceBetweenTwoPoints(pt1, pt2);
		}
		else if (this->m_DistributionFunction == DISTANCE_BETWEEN_TWO_RANDOM_POINTS)
		{
			Vector3D pt1((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt2((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]);

			ret = calcDistanceBetweenTwoPoints(pt1, pt2);
		}
		else if (this->m_DistributionFunction == SQRT_OF_AREA_OF_THREE_RANDOM_POINTS)
		{
			Vector3D pt1((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt2((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt3((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]);

			ret = sqrt(calcAreaInsideThreePoints(pt1, pt2, pt3));
		}
		else if (this->m_DistributionFunction == CUBE_ROOT_OF_VOLUME_OF_FOUR_RANDOM_POINTS)
		{
			Vector3D pt1((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt2((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt3((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]); ptItr++;
			Vector3D pt4((*ptItr)->coords[0], (*ptItr)->coords[1], (*ptItr)->coords[2]);

			ret = cbrt(calcVolumeInsideFourPoints(pt1, pt2, pt3, pt4));
		}

		return ret;
	}

	double PatchBasedShapeDistributionDescExtraction::createUnbiasedSurfaceSample(TriangularMesh* triMesh, const SinglePatch& patch) const
	{
		double ret = 0.0;
		return ret;
	}

	int PatchBasedShapeDistributionDescExtraction::getNumberOfRandomPointsForASample() const
	{
		int ret = 0;
		switch (this->m_DistributionFunction)
		{
			case ANGLE_BETWEEN_THREE_RANDOM_POINTS: ret = 3; break;
			case DISTANCE_BETWEEN_FIXED_AND_RANDOM_POINT: ret = 1; break;
			case DISTANCE_BETWEEN_TWO_RANDOM_POINTS: ret = 2; break;
			case SQRT_OF_AREA_OF_THREE_RANDOM_POINTS: ret = 3; break;
			case CUBE_ROOT_OF_VOLUME_OF_FOUR_RANDOM_POINTS: ret = 4; break;
		}

		return ret;
	}

	double PatchBasedShapeDistributionDescExtraction::calcAngleBetweenThreePoints(const Vector3D& pt1, const Vector3D& pt2, const Vector3D& pt3) const
	{
		Vector3D pt1ToPt2(pt1, pt2);
		Vector3D pt1ToPt3(pt1, pt3);
		return Vector3D::getAngleBetweenTwo(pt1ToPt2, pt1ToPt3);
	}

	double PatchBasedShapeDistributionDescExtraction::calcDistanceBetweenTwoPoints(const Vector3D& pt1, const Vector3D& pt2) const
	{
		return Vector3D::L2Distance(pt1, pt2);
	}

	double PatchBasedShapeDistributionDescExtraction::calcAreaInsideThreePoints(const Vector3D& pt1, const Vector3D& pt2, const Vector3D& pt3) const
	{
		Vector3D pt1ToPt2(pt1, pt2);
		Vector3D pt1ToPt3(pt1, pt3);
		const double pt1ToPt2Norm = pt1ToPt2.norm();
		const double pt1ToPt3Norm = pt1ToPt3.norm();

		//Gets simply the area between the three points. The reason why we did not call getAngleBetweenTwo is the efficiency
		//There is no need to calculate the norms of the vectors twice
		double angle = acos(Vector3D(pt1ToPt2 * (1.0 / pt1ToPt2Norm)) % Vector3D(pt1ToPt3 * (1.0 / pt1ToPt3Norm)));

		return 0.5 * pt1ToPt2Norm * pt1ToPt3Norm * sin(angle);
	}

	double PatchBasedShapeDistributionDescExtraction::calcVolumeInsideFourPoints(const Vector3D& pt1, const Vector3D& pt2, const Vector3D& pt3, const Vector3D& pt4) const
	{
		Vector3D pt1ToPt4(pt1, pt4);
		Vector3D pt2ToPt4(pt2, pt4);
		Vector3D pt3ToPt4(pt3, pt4);

		return abs(pt1ToPt4  % (pt2ToPt4 * pt3ToPt4)) / 6.0;
	}
}
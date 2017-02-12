#include "PatchBasedShapeDistributionDescExtraction.h"

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

	Result PatchBasedPerVertexFeatureExtraction::calcFeature(TriangularMesh* triMesh, const std::vector<PatchList>& listOfPatchLists, std::vector<LocalFeaturePtr>& outFeatures)
	{
		return TACore::TACORE_OK;
	}

	Result PatchBasedPerVertexFeatureExtraction::calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr)
	{
		return TACore::TACORE_OK;
	}
}
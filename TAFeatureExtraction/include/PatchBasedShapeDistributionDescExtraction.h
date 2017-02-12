#ifndef PATCH_BASED_SHAPE_DISTRIBUTION_DESC_EXTRACTION_H
#define PATCH_BASED_SHAPE_DISTRIBUTION_DESC_EXTRACTION_H

#include "PatchBasedPerVertexFeatureExtraction.h"

namespace TAFeaExt
{

	//PatchBasedSumOfCenterDistancesExtraction is a PatchBasedPerVertexFeatureExtraction
	//See PatchBasedPerVertexFeatureExtraction.h to answer the questions like what is a patch
	//This extraction tool extracts shape distributions from all patches
	//See http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf in order to have the opinion about what a shape distribution is
	//No extract methods. The objects of this class use the extract function of PatchBasedPerVertexFeatureExtraction
	class PatchBasedShapeDistributionDescExtraction : public PatchBasedPerVertexFeatureExtraction
	{
	public:
		virtual ~PatchBasedShapeDistributionDescExtraction();
		PatchBasedShapeDistributionDescExtraction();
		PatchBasedShapeDistributionDescExtraction(const PatchBasedShapeDistributionDescExtraction& other);

		//Type of the shape distribution function extracted
		enum ShapeDistributionFunction
		{
			ANGLE_BETWEEN_THREE_RANDOM_POINTS = 1,		//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf --> 4.1 A1
			DISTANCE_BETWEEN_FIXED_AND_RANDOM_POINT,	//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf --> 4.1 D1
			DISTANCE_BETWEEN_TWO_RANDOM_POINTS,			//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf --> 4.1 D2
			SQRT_OF_AREA_OF_THREE_RANDOM_POINTS,		//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf --> 4.1 D3
			CUBE_ROOT_OF_VOLUME_OF_FOUR_RANDOM_POINTS,	//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf --> 4.1 D4
		};

		//http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf 4.2, Importance of sampling method
		enum SamplingMethod
		{
			BIASED_VERTEX_SAMPLING = 1,					//Sensitive to changes in tessellation, but easy to implement
			UNBIASED_SURFACE_SAMPLING	
		};

		//Setters
		void setShapeDistributionFunction(const ShapeDistributionFunction& shapeDistributionFunction);
		void setSamplingMethod(const SamplingMethod& samplingMethod);
		void setSampleCountDecrementRatio(const float& sampleCountDecrementRatio);

		//Getters
		ShapeDistributionFunction getShapeDistributionFunction() const;
		SamplingMethod getSamplingMethod() const;
		float getSampleCountDecrementRatio() const;

		/**
		* @brief	Gets the type of the global descriptor extracted from each patch
		* @return	SHAPE_DISTRIBUTION_HISTOGRAM
		*/
		virtual TypeOfGlobalDescriptor getGlobalDescriptorType() const;

	protected:

		/**
		* @brief	Calculates patch based shape distribution histogram descriptor for a specified distribution function
		*			from extracted patches for ALL VERTICES
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
		* @brief	Calculates patch based shape distribution histogram descriptor for a specified distribution function 
		*			from extracted patches for a vertex
		*
		* @param	triMesh TriangularMesh
		* @param	id Id of the vertex whose feature is being extracted
		* @param	patchesVertexIds patches of a vertex
		* @param	[out] outFeaturePtr Output feature pointer
		* @return	TACORE_OK if everything goes fine
		*/
		virtual Result calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr);

	private:
		ShapeDistributionFunction	m_DistributionFunction;				//< Shape distribution function, see the comments in the enumaration
		SamplingMethod				m_SamplingMethod;					//< Sampling method, see the comments in the enumaration
		float						m_fSampleCountDecrementRatio;		//< For all patches, default number of samples is PatchSize * (PatchSize - 1) divided by 2. This ratio decrements this sample count for efficiency reasons																	//< Since number of samples dependson the patch size, the counts in histogram bins must be normalized with the sample size making it a probability density function.

		const static int			m_nNumberOfBins = 64;				//< Number of bins for all histograms for all patches
	};
}

#endif
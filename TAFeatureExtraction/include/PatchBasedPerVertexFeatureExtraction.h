#ifndef PATCH_BASED_PER_VERTEX_FEATURE_EXTRACTION_H
#define PATCH_BASED_PER_VERTEX_FEATURE_EXTRACTION_H

#include "TAFeatureExtraction.h"

namespace TAShape
{
	class TriangularMesh;
}

namespace TACore
{
	template<class T>
	class TAMatrix;
}

namespace TAFea
{
	class GeodesicDistanceMatrix;
};

namespace TAFeaExt
{
	class PatchBasedPerVertexFeatureExtraction : public PerVertexFeatureExtraction
	{
	public:
		typedef std::vector < int > SinglePatch;
		typedef std::vector< SinglePatch > PatchList;

		enum TypeOfGlobalDescriptor
		{
			SUM_EUC_DISTANCES_TO_CENTER = 1,	//< Euc Distance of each vertex inside the patch to the center of the patch
			SHAPE_DISTRIBUTION_HISTOGRAM		//< Histogram of the shape distribution: http://graphics.stanford.edu/courses/cs468-08-fall/pdf/osada.pdf	
		};

		virtual ~PatchBasedPerVertexFeatureExtraction();
		PatchBasedPerVertexFeatureExtraction();
		PatchBasedPerVertexFeatureExtraction(const PatchBasedPerVertexFeatureExtraction& other);

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr);

		/**
		* @brief	Gets the type of the global descriptor extracted from each patch
		*			Each derived class must implement the specify the type of the global descriptor
		* @return	SUM_EUC_DISTANCES_TO_CENTER --> Sum of the distances of all vertices to the center of the patch
		*/
		virtual TypeOfGlobalDescriptor getGlobalDescriptorType() const = 0;

		//Setters
		virtual void setMinGeodesicDistance(const float& minGeoDistance);
		virtual void setMaxGeodesicDistance(const float& maxGeoDistance);
		virtual void setNumberOfPatches(const int& noOfPatches);
		virtual void setGeodesicDistanceMatrix(TAFea::GeodesicDistanceMatrix* gdMatrix);

		//Getters
		virtual float getMinGeodesicDistance() const;
		virtual float getMaxGeodesicDistance() const;
		virtual int getNumberOfPatches() const;

	private:
		float m_fMinGeodesicRadius;									//< Minimum value for geodesic distance from which the smallest patch is created
		float m_fMaxGeodesicRadius;									//< Maximum value for geodesic distance from which the biggest patch is created
		int m_nNumberOfPatches;										//< Number of patches which will be the size of the output descriptor
		TAFea::GeodesicDistanceMatrix* m_pGeodeticDistanceMatrix;	//< Geodesic Distance Matrix used in the extraction. De-allocation is the responsibilty of the users of this class (No Getter)		

	protected:

		/**
		* @brief	Creates patches around a vertex for the radii between minimum radius and maximum radius from a triangular mesh
		*			Patches are considered as the list of vertex ids 
		*			A vertex is inside a patch when it is inside the region specified by the current radius
		*
		* @param	triMesh TriangularMesh
		* @param	vertexId The id of the vertex around which the patches will be created
		* @param	distanceMatrix Distance matrix from the which the vertex pair distances are obtained
		* @param	minRadius Minimum radius bounding the smallest patch
		* @param	maxRadius Maximum radius bounding the biggest patch
		* @param	noOfPatches Number of patches created around a single vertex
		* @param	[out] patchesVertexIds List of patches which contain the list of vertex ids
		*			
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if triMesh is NULL
		*/
		virtual Result createVertexPatches(TriangularMesh* triMesh, const int& vertexId, const TAMatrix<float>& distanceMatrix, const float& minRadius, const float& maxRadius, const int& noOfPatches, PatchList& patchesVertexIds);

		/**
		* @brief	Creates patches around a vertex for the radii between minimum radius and maximum radius from a triangular mesh
		*			Patches are considered as the list of vertex ids
		*			A vertex is inside a patch when it is inside the region specified by the current radius
		*
		* @param	triMesh TriangularMesh
		* @param	vertexId The id of the vertex around which the patches will be created
		* @param	distanceVector Distance vector of the specific vertex from the which the vertex pair distances are obtained
		* @param	minRadius Minimum radius bounding the smallest patch
		* @param	maxRadius Maximum radius bounding the biggest patch
		* @param	noOfPatches Number of patches created around a single vertex
		* @param	[out] patchesVertexIds List of patches which contain the list of vertex ids
		*
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if triMesh is NULL
		*/
		virtual Result createVertexPatches(TriangularMesh* triMesh, const int& vertexId, const std::vector<float>& distanceVector, const float& minRadius, const float& maxRadius, const int& noOfPatches, PatchList& patchesVertexIds);

		/**
		* @brief	Creates patches around all vertices for the radii between minimum radius and maximum radius from a triangular mesh
		*			Patches are considered as the list of vertex ids
		*			A vertex is inside a patch when it is inside the region specified by the current radius
		*
		* @param	triMesh TriangularMesh
		* @param	distanceMatrix Distance matrix from the which the vertex pair distances are obtained
		* @param	minRadius Minimum radius bounding the smallest patch
		* @param	maxRadius Maximum radius bounding the biggest patch
		* @param	noOfPatches Number of patches created around a single vertex
		* @param	[out] listOfPatchLists List of patch lists FOR ALL VERTICES
		*
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if triMesh is NULL
		*/
		virtual Result createPatches(TriangularMesh* triMesh, const TAMatrix<float>& distanceMatrix, const float& minRadius, const float& maxRadius, const int& noOfPatches, std::vector<PatchList>& listOfPatchLists);

		/**
		* @brief	Calculates features from extracted patches for ALL VERTICES
		*			All derived classes must implement this method
		*
		* @param	triMesh TriangularMesh
		* @param	listOfPatchLists patches of all vertices
		* @param	[out] outFeatures Output vector of features
		* @return	TACORE_OK if everything goes fine
		*/
		virtual Result calcFeature(TriangularMesh* triMesh, const std::vector<PatchList>& listOfPatchLists, std::vector<LocalFeaturePtr>& outFeatures) = 0;

		/**
		* @brief	Calculates features from extracted patches for a vertex
		*			All derived classes must implement this method
		*
		* @param	triMesh TriangularMesh
		* @param	id Id of the vertex whose feature is being extracted
		* @param	patchesVertexIds patches of a vertex
		* @param	[out] outFeaturePtr Output feature pointer
		* @return	TACORE_OK if everything goes fine
		*/
		virtual Result calcFeature(TriangularMesh* triMesh, const int& id, const PatchList& patchesVertexIds, LocalFeaturePtr& outFeaturePtr) = 0;
	};
}

#endif
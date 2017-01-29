#ifndef GEODESIC_DISTANCE_MATRIX_EXTRACTION_H
#define GEODESIC_DISTANCE_MATRIX_EXTRACTION_H

#include "TAFeatureExtraction.h"
#include "GeodesicDistanceMatrix.h"

namespace TAShape
{
	class TriangularMesh;
}

namespace TAFeaExt
{
	class GeodesicDistanceMatrixExtraction : GlobalFeatureExtraction
	{
	public:
		virtual ~GeodesicDistanceMatrixExtraction();
		GeodesicDistanceMatrixExtraction();
		GeodesicDistanceMatrixExtraction(const GeodesicDistanceMatrixExtraction& other);

		/**
		* @brief	Extracts geodesic distances for all vertices and saves them under a GlobalFeaturesPtr
		*			The type of the geodesic extracted can be checked using getGeodesicDistanceType
		*			and can be changed using setGeodesicDistanceType
		* @param	mesh PolygonMesh
		* @param	[out] outFeaturePtr Holds GeodesicDistanceMatrix
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS if mesh is not a triangular mesh
		* @return	TACORE_INVALID_OPERATION if the geodesic type is not implemented or not known
		*/
		virtual Result extract(PolygonMesh *mesh, GlobalFeaturePtr& outFeaturePtr);

		/**
		* @brief	Extracts geodesic distances for a single vertex and saves them under a vector
		*			The type of the geodesic extracted can be checked using getGeodesicDistanceType
		*			and can be changed using setGeodesicDistanceType
		* @param	triMesh TriangularMesh
		* @param	vid Index of vertex from which geodesic distances are extracted
		* @param	[out] distances Geodesic distances of all vertices from the given vertex
		* @return	TACORE_OK if everything goes fine
		* @return	TACORE_BAD_ARGS mesh is NULL
		* @return	TACORE_INVALID_OPERATION if the geodesic type is not implemented or not known
		*/
		virtual Result extract(TriangularMesh *triMesh, const int& vid, std::vector<float>& distances);

		//Setters
		void setGeodesicDistanceType(const GeodesicDistanceMatrix::DistanceType& type);

		//Getters
		GeodesicDistanceMatrix::DistanceType getGeodesicDistanceType() const;


	private:
		GeodesicDistanceMatrix::DistanceType m_MatrixDistanceType;

		/**
		* @brief	Extracts on edge geodesic distances for a single vertex and saves them under a vector
		* @param	triMesh TriangularMesh
		* @param	vid Index of vertex from which geodesic distances are extracted
		* @param	[out] distances Geodesic distances of all vertices from the given vertex
		* @return	TACORE_OK if everything goes fine
		*/
		Result extractOnEdgeGeodesics(TriangularMesh *triMesh, const int& vid, std::vector<float>& distances);
	};

}

#endif
#ifndef GEODESIC_DISTANCE_MATRIX_H
#define GEODESIC_DISTANCE_MATRIX_H

#include "TAFeature.h"
#include <core/TAMatrix.h>

namespace TAFea
{
	class GeodesicDistanceMatrix : public GlobalFeature
	{
	public:

		enum DistanceType
		{
			ON_EDGE_GEODESIC = 1,	//< Geodesic distance based on Djskstra's shortest path algorithm. Geodesic path only uses edges
			EXACT_GEODESIC			//< Geodesic distance implemented in http://hhoppe.com/geodesics.pdf
		};

		virtual ~GeodesicDistanceMatrix();
		GeodesicDistanceMatrix();
		GeodesicDistanceMatrix(const GeodesicDistanceMatrix::DistanceType& type);

	public:
		TACore::TAMatrix<float> m_GeoMatrix;
		DistanceType m_DistanceType;
	};
}



#endif
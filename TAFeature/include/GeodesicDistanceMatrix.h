#ifndef GEODESIC_DISTANCE_MATRIX_H
#define GEODESIC_DISTANCE_MATRIX_H

#include "TAFeature.h"
#include <core/TAMatrix.h>

namespace TAFea
{
	class GeodesicDistanceMatrix : public GlobalFeature
	{
	public:
		virtual ~GeodesicDistanceMatrix();
		GeodesicDistanceMatrix();

	public:
		TACore::TAMatrix<float> m_GeoMatrix;
	};
}



#endif
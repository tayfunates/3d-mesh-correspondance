#include "GeodesicDistanceMatrix.h"

namespace TAFea
{
	GeodesicDistanceMatrix::GeodesicDistanceMatrix()
	{
		this->m_DistanceType = ON_EDGE_GEODESIC;
	}

	GeodesicDistanceMatrix::GeodesicDistanceMatrix(const GeodesicDistanceMatrix::DistanceType& type)
	{
		this->m_DistanceType = type;
	}

	GeodesicDistanceMatrix::~GeodesicDistanceMatrix()
	{
	}
}
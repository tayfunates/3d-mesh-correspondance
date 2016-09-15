#include "AvgGeodesicDistance.h"

namespace TAFea
{
	AvgGeodesicDistance::AvgGeodesicDistance()
	{
		this->m_ID = -1;
		this->m_distance = 0.0f;
	}

	AvgGeodesicDistance::AvgGeodesicDistance(const int& vertexid)
	{
		this->m_ID = vertexid;
		this->m_distance = 0.0f;
	}

	AvgGeodesicDistance::AvgGeodesicDistance(const int& vertexid, const float& distance)
	{
		this->m_ID = vertexid;
		this->m_distance = distance;
	}

	AvgGeodesicDistance::AvgGeodesicDistance(const AvgGeodesicDistance& other)
	{
		this->m_ID = other.m_ID;
		this->m_distance = other.m_distance;
	}

	AvgGeodesicDistance::~AvgGeodesicDistance()
	{

	}
}
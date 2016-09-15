#ifndef AVG_GEODESIC_DISTANCE_H
#define AVG_GEODESIC_DISTANCE_H

#include "TAFeature.h"

namespace TAFea
{
	//Average Geodesic Distance descriptor for a single vertex in a mesh
	//AvgGeodesicDistance is a PerVertexFeature
	class AvgGeodesicDistance : public PerVertexFeature
	{
	public:
		//Public Constructors
		virtual ~AvgGeodesicDistance();
		AvgGeodesicDistance();
		AvgGeodesicDistance(const int& vertexid);
		AvgGeodesicDistance(const int& vertexid, const float& distance);
		AvgGeodesicDistance(const AvgGeodesicDistance& other);

	public:
		float m_distance;	//< Average Geodesic Distance value for a single vertex in a mesh
	
	};
}

#endif //AVG_GEODESIC_DISTANCE_H
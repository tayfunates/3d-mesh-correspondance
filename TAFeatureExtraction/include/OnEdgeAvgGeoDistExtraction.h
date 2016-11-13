#ifndef ON_EDGE_AVG_GEO_DIST_EXTRACTION_H
#define ON_EDGE_AVG_GEO_DIST_EXTRACTION_H

#include "TAFeatureExtraction.h"

using namespace TACore;
using namespace TAShape;
using namespace TAFea;

namespace TAFeaExt
{
	class OnEdgeAvgGeoDistExtraction : public PerVertexFeatureExtraction
	{
	public:
		virtual ~OnEdgeAvgGeoDistExtraction();
		OnEdgeAvgGeoDistExtraction();
		OnEdgeAvgGeoDistExtraction(const OnEdgeAvgGeoDistExtraction& other);

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr);
	};
}

#endif //ON_EDGE_AVG_GEO_DIST_EXTRACTION_H
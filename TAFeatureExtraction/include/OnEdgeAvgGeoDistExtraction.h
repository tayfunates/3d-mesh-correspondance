#ifndef ON_EDGE_AVG_GEO_DIST_EXTRACTION
#define ON_EDGE_AVG_GEO_DIST_EXTRACTION

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

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeature*>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeature **outFeature);
	};
}

#endif //ON_EDGE_AVG_GEO_DIST_EXTRACTION
#ifndef INTRINSIC_WAVE_DESC_EXTRACTION_H
#define INTRINSIC_WAVE_DESC_EXTRACTION_H

#include "TAFeatureExtraction.h"

using namespace TACore;
using namespace TAShape;
using namespace TAFea;

namespace TAFeaExt
{
	//Intrinsic Wave Descriptor for a single vertex in a mesh
	//Normalized sums of the distances inside disks of equi-distances encapsulating a vertex
	//See http://www.staff.uni-mainz.de/wandm/papers/eg11_plansac.pdf section 3.1 for more details
	//IntrinsicWaveDescExtraction is a PerVertexFeatureExtraction
	class IntrinsicWaveDescExtraction : public PerVertexFeatureExtraction
	{
	public:
		virtual ~IntrinsicWaveDescExtraction();
		IntrinsicWaveDescExtraction();
		IntrinsicWaveDescExtraction(const IntrinsicWaveDescExtraction& other);

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr);
	};
}

#endif
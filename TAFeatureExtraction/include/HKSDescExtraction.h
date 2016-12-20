#ifndef HEAT_KERNEÞ_SIGNATURE_DESC_EXTRACTION_H
#define HEAT_KERNEÞ_SIGNATURE_DESC_EXTRACTION_H

#include "TAFeatureExtraction.h"

namespace TAFeaExt
{
	//Heat Kernel Signature for a single vertex in a mesh
	//Technical Details of the feature is described in http://www.lix.polytechnique.fr/~maks/papers/hks.pdf
	//HKSDescExtraction is a PerVertexFeatureExtraction
	class HKSDescExtraction : public PerVertexFeatureExtraction
	{
	private:

	public:
		virtual ~HKSDescExtraction();
		HKSDescExtraction();
		HKSDescExtraction(const HKSDescExtraction& other);

		virtual Result extract(PolygonMesh *mesh, std::vector<LocalFeaturePtr>& outFeatures);
		virtual Result extract(PolygonMesh *mesh, const int& id, LocalFeaturePtr& outFeaturePtr);
	};
}

#endif

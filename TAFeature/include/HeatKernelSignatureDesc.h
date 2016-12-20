#ifndef HEAT_KERNEL_SIGNATURE_DESC_H
#define HEAT_KERNEL_SIGNATURE_DESC_H

#include "TAFeature.h"

namespace TAFea
{
	//Heat Kernel Signature for a single vertex in a mesh
	//Technical Details of the feature is described in http://www.lix.polytechnique.fr/~maks/papers/hks.pdf
	//Heat Kernel Signature is a PerVertexFeature
	class HeatKernelSignatureDesc : public PerVertexFeature
	{
	public:
		virtual ~HeatKernelSignatureDesc();
		HeatKernelSignatureDesc();
		HeatKernelSignatureDesc(const int& vertexid);

	private:

	};
}

#endif
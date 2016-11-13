#ifndef INTRINSIC_WAVE_DESC_H
#define INTRINSIC_WAVE_DESC_H

#include "TAFeature.h"

namespace TAFea
{
	//Intrinsic Wave Descriptor for a single vertex in a mesh
	//Normalized sums of the distances inside disks of equi-distances encapsulating a vertex
	//See http://www.staff.uni-mainz.de/wandm/papers/eg11_plansac.pdf section 3.1 for more details
	//IntrinsicWaveDesc is a PerVertexFeature
	class IntrinsicWaveDesc : public PerVertexFeature
	{
	public:
		virtual ~IntrinsicWaveDesc();
		IntrinsicWaveDesc();
		IntrinsicWaveDesc(const int& vertexid);

	private:

	};
}

#endif
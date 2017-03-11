#include "MultiParameterPerVertexDescriptorRun/PBSDParameterSet.h"
#include <core/StringUtil.h>
#include <iomanip>

PBSDParameterSet::PBSDParameterSet()
{
	m_fMinGeodesicDistance = 0;
	m_fMaxGeodesicDistance = 0;
	m_nNumberOfPatches = 0;
	m_nNumberOfBins = 0;
	m_nSampleCount = 0;
	m_nSamplingMethod = 0;
	m_nShapeDistributionFunction = 0;
}

PBSDParameterSet::PBSDParameterSet(float minGD, float maxGD, int noOfPatches, int noOfBins, int sampleCount, int samplingMethod, int shapeDistributionFunction)
{
	m_fMinGeodesicDistance = minGD;
	m_fMaxGeodesicDistance = maxGD;
	m_nNumberOfPatches = noOfPatches;
	m_nNumberOfBins = noOfBins;
	m_nSampleCount = sampleCount;
	m_nSamplingMethod = samplingMethod;
	m_nShapeDistributionFunction = shapeDistributionFunction;
}

PBSDParameterSet::~PBSDParameterSet()
{

}

std::vector<PBSDParameterSet> PBSDParameterSet::createPermutations(const PBSDParameterSet& minValues, const PBSDParameterSet& maxValues, const PBSDParameterSet& incrementValues)
{
	std::vector<PBSDParameterSet> result;
	for (float minGD = minValues.m_fMinGeodesicDistance; minGD <= maxValues.m_fMinGeodesicDistance; minGD += incrementValues.m_fMinGeodesicDistance)
	{
		for (float maxGD = minValues.m_fMaxGeodesicDistance; maxGD <= maxValues.m_fMaxGeodesicDistance; maxGD += incrementValues.m_fMaxGeodesicDistance)
		{
			for (int NOP = minValues.m_nNumberOfPatches; NOP <= maxValues.m_nNumberOfPatches; NOP += incrementValues.m_nNumberOfPatches)
			{
				for (int NOB = minValues.m_nNumberOfBins; NOB <= maxValues.m_nNumberOfBins; NOB += incrementValues.m_nNumberOfBins)
				{
					for (int SC = minValues.m_nSampleCount; SC <= maxValues.m_nSampleCount; SC += incrementValues.m_nSampleCount)
					{
						for (int SM = minValues.m_nSamplingMethod; SM <= maxValues.m_nSamplingMethod; SM += incrementValues.m_nSamplingMethod)
						{
							for (int SDF = minValues.m_nShapeDistributionFunction; SDF <= maxValues.m_nShapeDistributionFunction; SDF += incrementValues.m_nShapeDistributionFunction)
							{
								result.push_back(PBSDParameterSet(minGD, maxGD, NOP, NOB, SC, SM, SDF));
							}
						}
					}
				}
			}
		}
	}
	return result;
}

std::string PBSDParameterSet::getName(const std::string& meshName, const size_t& parameterSetId) const
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(10) << parameterSetId;
	const std::string name = meshName + "_" +
		ss.str() + "_" +
		"_MinGD_" + TACore::StringUtil::str(this->m_fMinGeodesicDistance) +
		"_MaxGD_" + TACore::StringUtil::str(this->m_fMaxGeodesicDistance) +
		"_NOP_" + TACore::StringUtil::str(this->m_nNumberOfPatches) +
		"_NOB_" + TACore::StringUtil::str(this->m_nNumberOfBins) +
		"_SC_" + TACore::StringUtil::str(this->m_nSampleCount) +
		"_SM_" + TACore::StringUtil::str(this->m_nSamplingMethod) +
		"_SDF_" + TACore::StringUtil::str(this->m_nShapeDistributionFunction);
	return name;
}

std::ostream& operator<<(std::ostream& o, const PBSDParameterSet& paramSet)
{
	o << "Min Geodesic Distance: " << paramSet.m_fMinGeodesicDistance << std::endl;
	o << "Max Geodesic Distance: " << paramSet.m_fMaxGeodesicDistance << std::endl;
	o << "Number of Patches: " << paramSet.m_nNumberOfPatches << std::endl;;
	o << "Number of Bins: " << paramSet.m_nNumberOfBins << std::endl;;
	o << "Sample Count: " << paramSet.m_nSampleCount << std::endl;;
	o << "Sampling Method: " << paramSet.m_nSamplingMethod << std::endl;;
	o << "Shape Distribution Function: " << paramSet.m_nShapeDistributionFunction << std::endl;;
	return o;
}
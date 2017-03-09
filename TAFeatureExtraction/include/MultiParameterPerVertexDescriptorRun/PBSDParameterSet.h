#ifndef PBSD_PARAMETER_SET_H
#define PBSD_PARAMETER_SET_H

#include <vector>
#include <iostream>
#include <string>

//Patch based shape distribution extraction parameter set
class PBSDParameterSet
{
public:
	/*
	* Constructors
	*/
	PBSDParameterSet();
	PBSDParameterSet(float minGD, float maxGD, int noOfPatches, int noOfBins, int sampleCount, int samplingMethod, int shapeDistributionFunction);

	/*
	* Destructor
	*/
	virtual ~PBSDParameterSet();

	//Parameters: See PatchBasedShapeDistributionDescExtraction
	float	m_fMinGeodesicDistance;
	float	m_fMaxGeodesicDistance;
	int		m_nNumberOfPatches;
	int		m_nNumberOfBins;
	int		m_nSampleCount;
	int		m_nSamplingMethod;
	int		m_nShapeDistributionFunction;

	//Ostream operator
	friend std::ostream& operator<<(std::ostream& o, const PBSDParameterSet& paramSet);

	std::string getName(const std::string& meshName) const;

	static std::vector<PBSDParameterSet> createPermutations(const PBSDParameterSet& minValues, const PBSDParameterSet& maxValues, const PBSDParameterSet& incrementValues);
};

#endif
#ifndef MULTI_PARAMETER_FEATURE_COMPARISON_MAIN
#define MULTI_PARAMETER_FEATURE_COMPARISON_MAIN

#include <map>
#include <vector>

namespace TAShape
{
	class TriangularMesh;
}

//Encapsulates a run method which is used as a script for comparing a source feature type with multiple parameters and a reference feature type
//Features of source types are read from several files created by MultiParameterFeatureExtractionMain
//Features of reference type is calculated inside
//Comparison takes a vertex as the reference
//Only static use is allowed
//No object creation of this class is allowed
class MultiParameterFeatureComparisonMain
{
private:

	//Private constructors and destructors
	MultiParameterFeatureComparisonMain();
	MultiParameterFeatureComparisonMain(const MultiParameterFeatureComparisonMain&);
	~MultiParameterFeatureComparisonMain();

public:

	//Run Result
	enum Result
	{
		MPFCR_OK = 0,
		MPFCR_COMMAND_LINE_PARSE_ERROR,
		MPFCR_MESH_LOAD_ERROR,
		MPFCR_FILE_ERROR
	};

	/**
	* @brief	Implements a main function which is used to create a comparison report between a source and reference feature types
	*			Features of source type are assumed to be extracted with multiple parameters and read from files
	*			On the contrary, features of reference type are extracted inside the run method
	*			Assumed to be called directly from a main function
	*
	* @param	argc Number of command line arguments
	* @param	argv List of command line arguments
	* @return	MPFCR_OK if everything goes fine
	* @return	MPFCR_COMMAND_LINE_PARSE_ERROR if command line cannot be parsed correctly
	* @return	MPFCR_MESH_LOAD_ERROR if mesh cannot be loaded correctly
	* @return	MPFCR_FILE_ERROR if the output file cannot be opened
	*/
	static Result run(int argc, char* argv[]);

private:

	/**
	* @brief	Read feature files and creates magnitude vectors for all permutation according to a reference vertex
	*			Permutation id must be in the first location of the split of the file name
	*			Split is done according to "_" character
	*			Example file name is given below:
	*				0000000064_MinGD_20_MaxGD_50_NOP_10_NOB_24_SC_128_SM_1_SDF_1.fea
	*			Magnitudes for each file are normalized (0 to 1) so that they can be compared with others
	*			Magnitudes represent the distances of all vertices to the reference vertex in a permutation file
	*
	* @param	featureFilePaths List of feature file paths
	* @param	refVertex Reference vertex which is used to assign magnitudes to other vertices
	* @return	Permutation id to magnitude vector mapping (For each permutation file, the vector of each vertices' magnitudes)
	*/
	static std::map<int, std::vector<double> > createPermutationIdToMagnitudesMapForPBSD(const std::vector<std::string>& featureFilePaths, const int& refVertex);

	/**
	* @brief	Read feature file names and create permutation id to parameter names and values map
	*			Permutation id must be in the first location of the split of the file name
	*			Split is done according to "_" character
	*			Example file name is given below:
	*				0000000064_MinGD_20_MaxGD_50_NOP_10_NOB_24_SC_128_SM_1_SDF_1.fea
	*			For the example above, 64 is the permutation id, so it is a key in the map
	*			The values of that key is the vector of pairs 
	*			Pairs contain the parameter name, such as MinGD
	*			and the parameter value, such as 20
	*
	* @param	featureFilePaths List of feature file paths
	* @return	Permutation id to parameter name-value pair lists
	*/
	static std::map<int, std::vector< std::pair< std::string, double > > > createPermutationIdToParameterValuesMap(const std::vector<std::string>& featureFilePaths);

	/**
	* @brief	Extracts average geodesic distances and creates magnitude vector according to a reference vertex
	*			Magnitudes are normalized (0 to 1) so that they can be compared with others
	*			Magnitudes represent the distances of all vertices to the reference vertex
	*
	* @param	triMesh TriangularMesh from which the magnitudes are calculated
	* @param	refVertex Reference vertex which is used to assign magnitudes to other vertices
	* @return	The vector of magnitudes
	*/
	static std::vector<double> createAGDMagnitudes(TAShape::TriangularMesh* triMesh, const int& refVertex);

	//TO DO: Comment when completed
	static Result generateComparisonResult(const std::vector<double>& referenceMagnitudes,
											const std::map<int, std::vector<double> >& permutationIdToSourceMagnitudesMap, 
											const std::map<int, std::vector< std::pair< std::string, double > > >& permutationIdToParametersMap, 
											const std::string& outFileName);
};

#endif
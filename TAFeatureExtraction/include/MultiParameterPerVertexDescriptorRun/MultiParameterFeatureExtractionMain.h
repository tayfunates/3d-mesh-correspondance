#ifndef MULTI_PARAMETER_FEATURE_EXTRACTION_MAIN
#define MULTI_PARAMETER_FEATURE_EXTRACTION_MAIN

//Encapsulates a run method which is used as a script for extracting some kind of per vertex feature with multiple parameters
//Only static use is allowed
//No object creation of this class is allowed
class MultiParameterFeatureExtractionMain
{
private:

	//Private constructors and destructors
	MultiParameterFeatureExtractionMain();
	MultiParameterFeatureExtractionMain(const MultiParameterFeatureExtractionMain&);
	~MultiParameterFeatureExtractionMain();

public:

	//Run Result
	enum Result
	{
		MPFER_OK = 0,
		MPFER_COMMAND_LINE_PARSE_ERROR,
		MPFER_MESH_LOAD_ERROR
	};

	/**
	* @brief	Implements a main function which is used to create per vertex features from a mesh with several parameters
	*			Assumed to be called directly from a main function
	*
	* @param	argc Number of command line arguments
	* @param	argv List of command line arguments
	* @return	MPFER_OK if everything goes fine
	* @return	MPFER_COMMAND_LINE_PARSE_ERROR if command line cannot be parsed correctly
	* @return	MPFER_MESH_LOAD_ERROR if mesh cannot be loaded correctly
	*/
	static Result run(int argc, char* argv[]);
};

#endif
#include <core/Defs.h>
#include <iostream>
#include <string>
#include <MultiParameterPerVertexDescriptorRun/MultiParameterFeatureExtractionMain.h>
#include <MultiParameterPerVertexDescriptorRun/MultiParameterFeatureComparisonMain.h>

int mainRet(int ret, std::string message)
{
	std::cout << "Message: " << message << std::endl;
#ifdef TACORE_IS_WINDOWS
	std::cout << "Press Enter to Exit The App..." << std::endl;
	system("pause");
#endif
	return ret;
}

int main(int argc, char* argv[])
{
	//0 --> MultiParameterFeatureExtractionMain
	//1 --> MultiParameterFeatureComparisonMain
	const int mainMode = 0;

	std::string runResultMessage = "Main Test Successfully Ended";
	int runResultCode = 1;

	if (mainMode == 0)
	{
		MultiParameterFeatureExtractionMain::Result res = MultiParameterFeatureExtractionMain::run(argc, argv);
		if (res == MultiParameterFeatureExtractionMain::MPFER_COMMAND_LINE_PARSE_ERROR)
		{
			runResultMessage = "Command Line Cannot Be Parsed Correctly";
			runResultCode = -1;
		}
		else if (res == MultiParameterFeatureExtractionMain::MPFER_MESH_LOAD_ERROR)
		{
			runResultMessage = "Mesh Cannot Be Loaded Correctly";
			runResultCode = -1;
		}
	}

	else if (mainMode == 1)
	{
		MultiParameterFeatureComparisonMain::Result res = MultiParameterFeatureComparisonMain::run(argc, argv);
		if (res == MultiParameterFeatureComparisonMain::MPFCR_COMMAND_LINE_PARSE_ERROR)
		{
			runResultMessage = "Command Line Cannot Be Parsed Correctly";
			runResultCode = -1;
		}
		else if (res == MultiParameterFeatureComparisonMain::MPFCR_MESH_LOAD_ERROR)
		{
			runResultMessage = "Mesh Cannot Be Loaded Correctly";
			runResultCode = -1;
		}
		else if (res == MultiParameterFeatureComparisonMain::MPFCR_FILE_ERROR)
		{
			runResultMessage = "Output File Cannot Be Created Correctly";
			runResultCode = -1;
		}
	}

	return mainRet(runResultCode, runResultMessage);
}
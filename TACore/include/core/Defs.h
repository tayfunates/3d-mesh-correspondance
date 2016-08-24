#ifndef TACORE_DEFS_H
#define TACORE_DEFS_H

// Minimum required headers
#include <string>

namespace TACore
{
	/**
	* @brief	Return values of TACore functions.
	*/
	enum Result
	{
		TACORE_OK = 0,				///< Success
		TACORE_ERROR,				///< Generic error
		TACORE_BAD_ARGS,			///< Some args are invalid
		TACORE_INVALID_VALUE,		///< A value is invalid
		TACORE_INVALID_OPERATION,	///< Operation is not allowed
		TACORE_MEMORY_ERROR,		///< A memory problem occurred (most probably out of memory)
		TACORE_FILE_ERROR,			///< A file error occurred (most probably file not found or supported)
		TACORE_DEVICE_NOT_PRESENT,	///< A requested system device (i.e. audio capture or playback) not present 
		TACORE_NULL_POINTER,		///< NULL pointer
		TACORE_TIMEOUT				///< Timeout
	};

	// Depreciation 
	// ----------------------------------------------------------------------------------------
#ifdef _MSC_VER
#define TACORE_DEPRECATED __declspec(deprecated)
#else
#define TACORE_DEPRECATED __attribute__((deprecated))
#endif


	// These macros return TACore::Result in case of assertion failure
	// ----------------------------------------------------------------------------------------
#define TACORE_CHECK_ARGS(EXPRESSION)			if (!(EXPRESSION)) { return TACore::TACORE_BAD_ARGS; } 
#define TACORE_CHECK_PRECONDITION(EXPRESSION)	if (!(EXPRESSION)) { return TACore::TACORE_INVALID_OPERATION; } 
#define TACORE_CHECK_RESULT(EXPRESSION)			{ TACore::Result _tacOre_result_=(EXPRESSION); if ( _tacOre_result_ !=TACore::TACORE_OK) { return _tacOre_result_; } }
#define TACORE_CHECK_PTR(PTR)					if (!(PTR)) return TACore::TACORE_NULL_POINTER; 
#define TACORE_CHECK(EXPRESSION)					if (!(EXPRESSION)) return TACore::TACORE_ERROR;  

	// Checked deleters
	// ----------------------------------------------------------------------------------------
#define TACORE_SAFE_DELETE(PTR)					if (PTR) {delete (PTR); (PTR) = NULL;}
#define TACORE_SAFE_DELETE_ARRAY(PTR)			if (PTR) {delete [] (PTR); (PTR) = NULL;}
#define TACORE_SAFE_FREE(PTR)					if (PTR) {free (PTR); (PTR) = NULL;} 


#ifndef MAX
#	define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#ifndef MIN
#	define MIN(x,y) ((x)<(y)?(x):(y))
#endif

	// Platform Checkers
	// ----------------------------------------------------------------------------------------

#if (defined(_WIN32) || defined(__cplusplus_winrt))
#	define TACORE_IS_WINDOWS
#endif


#if (defined(__linux__))
#	define TACORE_IS_LINUX
#endif

#if defined(_MSC_VER)
#	if _MSC_VER >= 1800 
#		define TACORE_IS_CPP11
#	endif
#elif __cplusplus > 199711L
#	define TACORE_IS_CPP11
#endif

} // namespace TACore

#endif

#ifndef CORE_DEFS_H
#define CORE_DEFS_H

namespace TAShape
{

#ifdef _MSC_VER
#define DLL_EXPORT	_declspec(dllexport)
#define DLL_IMPORT	_declspec(dllimport)
#else 	//not _MSC_VER
#define DLL_EXPORT  __attribute__ ((visibility ("default")))
#define DLL_IMPORT
#endif  //_MSC_VER

	// C++11 directives
	// ----------------------------------------------------------------------------
	// Use override as much as possible
#if defined( _MSC_VER ) && ( _MSC_VER >= 1600 )
	// Visual Studio 2010 or greater supports override
#define OVERRIDE override
#define FINAL final
#else
#define OVERRIDE
#define FINAL
#endif
}

#endif




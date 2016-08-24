#ifndef TACORE_PATH_UTIL_H
#define TACORE_PATH_UTIL_H

#include <string>
#include <vector>

namespace TACore
{

	/**
	* @brief	Encapsulates path manipulation functions.
	*/
	class PathUtil
	{
	public:

		/**
		* @brief	Query if 'path' the root or not.
		* @param	path	Full pathname.
		* @return	true if path is "/" in Linux, "DR:/" or "DR:\" where DR is a drive name in Windows eg.C:/, false otherwise.
		*/
		static bool isRootFolder(const std::string& path);

		/**
		* @brief	Query if 'path' the root or not.
		* @param	path	Full pathname.
		* @return	true if path is "/" in Linux, "DR:/" or "DR:\" where DR is a drive name in Windows eg.C:/, false otherwise.
		*/
		static bool isRootFolder(const std::wstring& path);

		/**
		* Adds an extension to a file name (the file name itself should be without extensions. i.e. addExtension("test","wav") ==> test.wav
		* \param fileName the file name
		* \param extension the extension to add to the fileName
		* \return the created file name
		*/
		static const std::string addExtension(const std::string& fileName, const std::string& extension);


		/**
		* Adds an extension to a file name (the file name itself should be without extensions. i.e. addExtension("test","wav") ==> test.wav
		* \param fileName the file name
		* \param extension the extension to add to the fileName
		* \return the created file name
		*/
		static const std::wstring addExtension(const std::wstring& fileName, const std::wstring& extension);


		/**
		* removes the last folder from the folder list (may remove the file name if the is no folders)
		* i.e. C:\test\abc\b -> C:\test\abc
		* \param pathName the initial path name
		* \ return the reduced path name
		*/
		static const std::wstring removeCanonicalLast(const std::wstring& pathName);

		/**
		* removes the last folder from the folder list (may remove the file name if the is no folders)
		* i.e. C:\test\abc\b -> C:\test\abc
		* \param pathName the initial path name
		* \ return the reduced path name
		*/
		static const std::string removeCanonicalLast(const std::string& pathName);

		/**
		* @brief	Removes the first folder from the folder list (may remove the file name if the is no folders)
		* i.e. C:\test\abc\b -> test\abc\b
		* \param pathName the initial path name
		* \ return the reduced path name
		*/
		static const std::string removeCanonicalFirst(const std::string& pathName);
		/**
		* @brief	Removes the first folder from the folder list (may remove the file name if the is no folders)
		* i.e. C:\test\abc\b -> test\abc\b
		* \param pathName the initial path name
		* \ return the reduced path name
		*/
		static const std::wstring removeCanonicalFirst(const std::wstring& pathName);



		/**
		* removes file extension from file name , i.e. test.wav ==> test
		* \param fileName the fileName with extension
		* \return the file name without extension
		*/
		static const std::string stripExtension(const std::string& fileName);

		/**
		* removes file extension from file name , i.e. test.wav ==> test
		* \param fileName the fileName with extension
		* \return the file name without extension
		*/
		static const std::wstring stripExtension(const std::wstring& fileName);



		/**
		* returns file extension from file name , i.e. test.wav ==> test
		* \param fileName the fileName with extension
		* \return the file extension
		*/
		static const std::string getExtension(const std::string& fileName);


		/**
		* returns file extension from file name , i.e. test.wav ==> test
		* \param fileName the fileName with extension
		* \return the file extension
		*/
		static const std::wstring getExtension(const std::wstring& fileName);



		/**
		* Joins a folder name with a file name adding the / as required.
		* \param path the folder.
		* \param fileName the file name to add to the path.
		* \return the joined path
		*/
		static const std::string joinPath(const std::string& path, const std::string& fileName);

		/**
		* Joins a folder name with a file name adding the / as required.
		* \param path the folder.
		* \param fileName the file name to add to the path.
		* \return the joined path
		*/
		static const std::wstring joinPath(const std::wstring& path, const std::wstring& fileName);



		/**
		* extracts file name from the path.
		* \param fullPath the file name with the path.
		* \return the file name without the path.
		*/
		static const std::string getFileNameFromPath(const std::string& fullPath);


		/**
		* extracts file name from the path.
		* \param fullPath the file name with the path.
		* \return the file name without the path.
		*/
		static const std::wstring getFileNameFromPath(const std::wstring& fullPath);


		/**
		* extracts folder name from the path.
		* \param fullPath the file name with the path.
		* \return the folder name .
		*/
		static const std::string getFolderNameFromPath(const std::string& fullPath);


		/**
		* extracts folder name from the path.
		* \param fullPath the file name with the path.
		* \return the folder name .
		*/
		static const std::wstring getFolderNameFromPath(const std::wstring& fullPath);


		/**
		* @brief	Query if 'path' is absolute path.
		* @param	path	Full pathname.
		* @return	true if absolute path (starts with '/' or '\' or ':' or '~' ), false if not.
		*/
		static bool isAbsolutePath(const std::string& path);

		/**
		* @brief	Query if 'path' is absolute path.
		* @param	path	Full pathname.
		* @return	true if absolute path (starts with '/' or '\' or ':' or '~' ), false if not.
		*/
		static bool isAbsolutePath(const std::wstring& path);




		/**
		* returns the current folder (i.e. pwd or cd)
		* \return the current folder
		*/
		static const std::string getCurrentDirectory();

		/**
		* returns the current folder (i.e. pwd or cd)
		* \return the current folder
		*/
		static const std::wstring getCurrentDirectoryW();

		/**
		* Takes a folder path that ends with a separator
		* fills the vector entries with the files/foldes under the path
		* by filtering with the parameters ext, getFolders and getHiddens
		*/
		static const void getDirectoryEntries(const std::string& path, std::vector<std::string>& entries, const std::string& ext = "", bool getFolders = false, bool getHiddens = false);
	};

} //namespace TACore

#endif

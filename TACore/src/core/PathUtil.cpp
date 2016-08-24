#include "core/PathUtil.h"

#include <algorithm>

#if defined _WIN32 || defined _WIN64
#include <direct.h>
#include <windows.h>
#define PATH_SEPERATOR_CHAR '\\'
#define ALT_PATH_SEPERATOR_CHAR '/'
#define PATH_SEPERATOR  "\\"
#define PATH_SEPERATORW L"\\"
#define ALT_PATH_SEPERATOR "/"
#define ALT_PATH_SEPERATORW L"/"
#define ALL_PATH_SEPERATORS "\\/"
#define ALL_PATH_SEPERATORSW L"\\/"
#define HOME_FOLDER "~"
#define HOME_FOLDERW L"~"
#define DRIVE_SEPARATOR ":"
#define DRIVE_SEPARATORW L":"
#else 
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h> 
#define PATH_SEPERATOR_CHAR '/'
#define ALT_PATH_SEPERATOR_CHAR '\\'
#define PATH_SEPERATOR  "/"
#define PATH_SEPERATORW L"/"
#define ALT_PATH_SEPERATOR  "\\"
#define ALT_PATH_SEPERATORW L"\\"
#define ALL_PATH_SEPERATORS "\\/"
#define ALL_PATH_SEPERATORSW L"\\/"
#define HOME_FOLDER "~"
#define HOME_FOLDERW L"~"
#define DRIVE_SEPARATOR ":"
#define DRIVE_SEPARATORW L":"
#define _MAX_PATH 1024
#endif

namespace TACore
{

	bool PathUtil::isRootFolder(const std::string& path)
	{
		if (path.empty()) return false;

		//Windows Root Folder
#if defined _WIN32 || defined _WIN64
		size_t ds_pos = path.find_first_of(DRIVE_SEPARATOR);
		size_t ps_pos = path.find_last_of(ALL_PATH_SEPERATORS);
		if (path == PATH_SEPERATOR || path == ALT_PATH_SEPERATOR) {
			return true;
		}
		else if (ds_pos != std::string::npos && ps_pos != std::string::npos) {
			if ((ds_pos == ps_pos - 1) && (ps_pos == path.length() - 1)) {
				return true;
			}
		}
		//Linux Root Folder
#else
		if (path == PATH_SEPERATOR) {
			return true;
		}
#endif
		return false;
	}

	bool PathUtil::isRootFolder(const std::wstring& path)
	{
		if (path.empty()) return false;

		//Windows Root Folder
#if defined _WIN32 || defined _WIN64
		size_t ds_pos = path.find_first_of(DRIVE_SEPARATORW);
		size_t ps_pos = path.find_last_of(ALL_PATH_SEPERATORSW);
		if (path == PATH_SEPERATORW || path == ALT_PATH_SEPERATORW) {
			return true;
		}
		else if (ds_pos != std::string::npos && ps_pos != std::string::npos) {
			if ((ds_pos == ps_pos - 1) && (ps_pos == path.length() - 1)) {
				return true;
			}
		}
		//Linux Root Folder
#else
		if (path == PATH_SEPERATORW) {
			return true;
		}
#endif
		return false;
	}

	const std::string PathUtil::addExtension(const std::string& fileName, const std::string& extension)
	{
		if (extension.length() > 0){
			return fileName + "." + extension;
		}
		else{
			return fileName;
		}
	}

	const std::wstring PathUtil::addExtension(const std::wstring& fileName, const std::wstring& extension)
	{
		if (extension.length() > 0){
			return fileName + L"." + extension;
		}
		else{
			return fileName;
		}

	}

	const std::wstring PathUtil::removeCanonicalLast(const std::wstring& pathName)
	{
		if (isRootFolder(pathName) || pathName.empty()) {
			return pathName;
		}
		size_t pos = pathName.find_last_of(ALL_PATH_SEPERATORSW);
		//if there is path seperator at the end, ignore it
		std::wstring localPathName = ((pos == std::string::npos) || (pos != pathName.length() - 1)) ? pathName : pathName.substr(0, pathName.length() - 1);

		pos = localPathName.find_last_of(ALL_PATH_SEPERATORSW);

		if (pos != std::string::npos) {
			std::wstring remPathName(localPathName.substr(0, pos + 1));
			if (isRootFolder(remPathName)) {
				return remPathName;
			}
			else {
				return localPathName.substr(0, pos);
			}
		}
		else {
			size_t ds_pos = localPathName.find_first_of(DRIVE_SEPARATORW);
			if (ds_pos != std::string::npos) {
				return localPathName.substr(0, ds_pos + 1);
			}
		}
		return localPathName;
	}

	const std::string PathUtil::removeCanonicalLast(const std::string& pathName)
	{
		if (isRootFolder(pathName) || pathName.empty()) {
			return pathName;
		}
		size_t pos = pathName.find_last_of(ALL_PATH_SEPERATORS);
		//if there is path seperator at the end, ignore it
		std::string localPathName = ((pos == std::string::npos) || (pos != pathName.length() - 1)) ? pathName : pathName.substr(0, pathName.length() - 1);

		pos = localPathName.find_last_of(ALL_PATH_SEPERATORS);

		if (pos != std::string::npos) {
			std::string remPathName(localPathName.substr(0, pos + 1));
			if (isRootFolder(remPathName)) {
				return remPathName;
			}
			else {
				return localPathName.substr(0, pos);
			}
		}
		else {
			size_t ds_pos = localPathName.find_first_of(DRIVE_SEPARATOR);
			if (ds_pos != std::string::npos) {
				return localPathName.substr(0, ds_pos + 1);
			}
		}
		return localPathName;
	}


	const std::string PathUtil::removeCanonicalFirst(const std::string& pathName)
	{
		if (pathName.empty()){
			return "";
		}




		//First check for drive
		size_t pos = pathName.find_first_of(HOME_FOLDER);
		if (pos != std::string::npos){
			return pathName.substr(pos + 1);
		}

		pos = pathName.find_first_of(DRIVE_SEPARATOR);
		if (pos != std::string::npos){
			//After the drive separator, check for the character at pos+1 and skip if it is one of the \/
			size_t innerPos = pos;
			do
			{
				pos = pathName.find_first_of(ALL_PATH_SEPERATORS, innerPos + 1);

				if (pos == innerPos + 1)
				{
					//found. skip
					innerPos = pos;
				}
				else{
					break;
				}
			} while (true);


			return pathName.substr(innerPos + 1);

		}

		pos = pathName.find_first_of(ALL_PATH_SEPERATORS);
		if (pos != std::string::npos) {
			return pathName.substr(pos + 1);
		}

		return "";
	}

	const std::wstring PathUtil::removeCanonicalFirst(const std::wstring& pathName)
	{
		if (pathName.empty()) {
			return L"";
		}




		//First check for drive
		size_t pos = pathName.find_first_of(HOME_FOLDERW);
		if (pos != std::wstring::npos){
			return pathName.substr(pos + 1);
		}

		pos = pathName.find_first_of(DRIVE_SEPARATORW);
		if (pos != std::wstring::npos){
			//After the drive separator, check for the character at pos+1 and skip if it is one of the \/
			size_t innerPos = pos;
			do
			{
				pos = pathName.find_first_of(ALL_PATH_SEPERATORSW, innerPos + 1);

				if (pos == innerPos + 1)
				{
					//found. skip
					innerPos = pos;
				}
				else{
					break;
				}
			} while (true);


			return pathName.substr(innerPos + 1);

		}

		pos = pathName.find_first_of(ALL_PATH_SEPERATORSW);
		if (pos != std::wstring::npos) {
			return pathName.substr(pos + 1);
		}

		return L"";
	}




	const std::string PathUtil::stripExtension(const std::string& fileName)
	{
		size_t pos = fileName.rfind('.');
		if (pos != std::string::npos)
		{
			return fileName.substr(0, pos);
		}
		else{
			return fileName;
		}

	}

	const std::wstring PathUtil::stripExtension(const std::wstring& fileName)
	{
		size_t pos = fileName.rfind('.');
		if (pos != std::string::npos)
		{
			return fileName.substr(0, pos);
		}
		else{
			return fileName;
		}
	}


	const std::string PathUtil::getExtension(const std::string& fileName)
	{
		//Get the extension of the last item in the canonical list.
		size_t lastElementPos = fileName.find_last_of(ALL_PATH_SEPERATORS);
		size_t searchStartPos = (lastElementPos == std::string::npos) ? 0 : lastElementPos;	//as we use -1
		size_t pos = fileName.rfind('.');

		if (pos != std::string::npos && pos >= searchStartPos)	//Actually >= is not the expected 
		{
			return fileName.substr(pos + 1, std::string::npos);
		}
		else{
			return "";
		}
	}


	const std::wstring PathUtil::getExtension(const std::wstring& fileName)
	{
		//Get the extension of the last item in the canonical list.


		size_t lastElementPos = fileName.find_last_of(ALL_PATH_SEPERATORSW);
		size_t searchStartPos = (lastElementPos == std::wstring::npos) ? 0 : lastElementPos;	//as we use -1
		size_t pos = fileName.rfind('.');

		if (pos != std::wstring::npos && pos >= searchStartPos)
		{
			return fileName.substr(pos + 1, std::wstring::npos);
		}
		else{
			return L"";
		}
	}


	const std::string PathUtil::joinPath(const std::string& path, const std::string& fileName)
	{
		if (isAbsolutePath(fileName)) return fileName;

		if ((path.length() == 0) || (fileName.length() == 0)) {
			return path + fileName;
		}

		size_t pos = path.find_last_of(ALL_PATH_SEPERATORS);
		if (pos != path.size() - 1){
			//Some OSes allow {DrivePath}:{fileName}, 
#if defined _WIN32 || defined _WIN64
			size_t driveLetterSeparatorPos = path.find(':');
			//if (driveLetterSeparatorPos!=std::string::npos){
			if (driveLetterSeparatorPos != std::string::npos && driveLetterSeparatorPos == path.size() - 1){
				return path + fileName;
			}
			else{
				return path + PATH_SEPERATOR + fileName;
			}
#else
			return path + PATH_SEPERATOR + fileName;
#endif
		}
		else{
			return path + fileName;
		}
	}


	const std::wstring PathUtil::joinPath(const std::wstring& path, const std::wstring& fileName)
	{
		if (isAbsolutePath(fileName)) return fileName;
		if ((path.length() == 0) || (fileName.length() == 0)) {
			return path + fileName;
		}
		size_t pos = path.find_last_of(ALL_PATH_SEPERATORSW);

		if (pos != path.size() - 1) {
			//Some OSes allow {DrivePath}:{fileName}, 
#if defined _WIN32 || defined _WIN64
			size_t driveLetterSeparatorPos = path.find(':');
			//if (driveLetterSeparatorPos!= std::string::npos && ){
			if (driveLetterSeparatorPos == path.size() - 1){
				return path + fileName;
			}
			else{
				return path + PATH_SEPERATORW + fileName;
			}
#else
			return path + PATH_SEPERATORW + fileName;
#endif
		}
		else{
			return path + fileName;
		}
	}


	const std::string PathUtil::getFileNameFromPath(const std::string& fullPath)
	{
		size_t pos = fullPath.find_last_of(ALL_PATH_SEPERATORS);
		if (pos != std::string::npos){
			return fullPath.substr(pos + 1);
		}
		else{
			//the file name may be DriveLetter:filename on certain OSes. The : is a valid filename character in others.
#if defined _WIN32 || defined _WIN64
			size_t driveLetterSeparatorPos = fullPath.find(':');
			if (driveLetterSeparatorPos != std::string::npos){
				return fullPath.substr(driveLetterSeparatorPos + 1);
			}
			else{
				return fullPath;
			}
#else
			return fullPath;
#endif
		}
	}


	const std::wstring PathUtil::getFileNameFromPath(const std::wstring& fullPath)
	{
		size_t pos = fullPath.find_last_of(ALL_PATH_SEPERATORSW);
		if (pos != std::wstring::npos){
			return fullPath.substr(pos + 1);
		}
		else{
#if defined _WIN32 || defined _WIN64
			size_t driveLetterSeparatorPos = fullPath.find(':');
			if (driveLetterSeparatorPos != std::wstring::npos){
				return fullPath.substr(driveLetterSeparatorPos + 1);
			}
			else{
				return fullPath;
			}
#else
			return fullPath;
#endif
		}
	}


	const std::string PathUtil::getFolderNameFromPath(const std::string& fullPath)
	{
		size_t pos = fullPath.find_last_of(ALL_PATH_SEPERATORS);
		if (pos != std::string::npos){
			return fullPath.substr(0, pos);
		}
		else{
			return fullPath;
		}
	}

	const std::wstring PathUtil::getFolderNameFromPath(const std::wstring& fullPath)
	{
		size_t pos = fullPath.find_last_of(ALL_PATH_SEPERATORSW);
		if (pos != std::string::npos){
			return fullPath.substr(0, pos);
		}
		else{
			return fullPath;
		}
	}


	// Windows allows -->  C:a.txt. this is also interpreted as an absolute path.
	bool PathUtil::isAbsolutePath(const std::string& path)
	{
		if (path.empty()) return false;
		size_t pos = path.find_first_of(HOME_FOLDER);
		if (pos == 0) return true;

		pos = path.find_first_of(DRIVE_SEPARATOR);
		if (pos != std::string::npos) return true;

		pos = path.find_first_of(ALL_PATH_SEPERATORS);
		if (pos == 0) return true;

		return false;
	}

	// Windows allows -->  C:a.txt. this is also interpreted as an absolute path.
	bool PathUtil::isAbsolutePath(const std::wstring& path)
	{
		if (path.empty()) return false;
		size_t pos = path.find_first_of(HOME_FOLDERW);
		if (pos == 0) return true;

		pos = path.find_first_of(DRIVE_SEPARATORW);
		if (pos != std::string::npos) return true;

		pos = path.find_first_of(ALL_PATH_SEPERATORSW);
		if (pos == 0) return true;

		return false;
	}

	const std::string PathUtil::getCurrentDirectory()
	{
		char buffer[_MAX_PATH + 1];

#if defined _WIN32 || defined _WIN64
		if (_getcwd(buffer, _MAX_PATH) == NULL){
#else
		if (getcwd(buffer, _MAX_PATH) == NULL){
#endif
			return "";
		}
		else{
			return std::string(buffer);
		}
		}


	const std::wstring PathUtil::getCurrentDirectoryW()
	{
		//Windows only!
#if defined _WIN32 || defined _WIN64
		wchar_t buffer[_MAX_PATH + 1];
		if (_wgetcwd(buffer, _MAX_PATH) == NULL){
			return L"";
		}
		else{
			return std::wstring(buffer);
		}
#else
		return L"";
#endif
	}

	const void PathUtil::getDirectoryEntries(const std::string& path, std::vector<std::string>& entries, const std::string& ext, bool getFolders, bool getHiddens) {
		if (ext != "") {
			getFolders = false;
		}
#if defined _WIN32 || defined _WIN64
		WIN32_FIND_DATAA search_data;

		memset(&search_data, 0, sizeof(WIN32_FIND_DATAA));

		HANDLE handle = FindFirstFileA((path + "*").c_str(), &search_data);

		while (handle != INVALID_HANDLE_VALUE)
		{
			std::string d_entry = std::string(search_data.cFileName);
			std::string full_path = path + d_entry;
			DWORD attr = GetFileAttributesA(full_path.c_str());

			if (d_entry == ".." || d_entry == "." || ((ext != "") && getExtension(d_entry) != ext)) {
				if (FindNextFileA(handle, &search_data) == FALSE)
					break;
				continue;
			}

			if (getFolders && getHiddens) {
				entries.push_back(full_path);
			}
			else if (getFolders && !getHiddens) {
				if (!(attr & FILE_ATTRIBUTE_HIDDEN)) {
					entries.push_back(full_path);
				}
			}
			else if (!getFolders && getHiddens) {
				if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
					entries.push_back(full_path);
				}
			}
			else {
				if (!(attr & FILE_ATTRIBUTE_DIRECTORY) && !(attr & FILE_ATTRIBUTE_HIDDEN)) {
					entries.push_back(full_path);
				}
			}

			if (FindNextFileA(handle, &search_data) == FALSE)
				break;
		}
		FindClose(handle);
#else
		DIR *dir = opendir(path.c_str());
		if (!dir)
		{
			return;
		}
		struct dirent *ent;

		while ((ent = readdir(dir)) != NULL)
		{
			std::string d_entry = ent->d_name;
			if (d_entry == ".." || d_entry == "." || ((ext != "") && getExtension(d_entry) != ext)) continue; // .. and . are also directory entries
			std::string full_path = path + d_entry;

			struct stat st;
			stat(full_path.c_str(), &st);
			if (getFolders && getHiddens) {
				entries.push_back(full_path);
			}
			else if (getFolders && !getHiddens) {
				if (d_entry[0] != '.') {
					entries.push_back(full_path);
				}
			}
			else if (!getFolders && getHiddens) {
				if (!(S_ISDIR(st.st_mode))) {
					entries.push_back(full_path);
				}
			}
			else {
				if (!(S_ISDIR(st.st_mode)) && (d_entry[0] != '.')) {
					entries.push_back(full_path);
				}
			}
		}
#endif
		std::sort(entries.begin(), entries.end());
	}

	} // namespace TACore 

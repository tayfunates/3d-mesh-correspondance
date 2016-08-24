#ifndef TACORE_STRING_UTIL_H
#define TACORE_STRING_UTIL_H

#include <sstream>
#include <vector>

namespace TACore
{

	/**
	* @brief	Encapsulates narrow/wide and UTF8 string manipulation functions.
	*/
	class StringUtil
	{
	public:

		/**
		* @brief	Gets turkish locale name.
		* @return	Turkish locale name ("turkish" or "tr_TR.utf8").
		*/
		static std::string	getTurkishLocaleName();


		/**
		* @brief	Counts the number of occurences of character 'c' in string 's'.
		* @param	s	The string.
		* @param	c	The character to be counted.
		* @return	The number of 'c's in the string.
		*/
		static size_t	count(const std::string &s, char c);

		/**
		* @brief	Counts the number of occurences of character 'c' in string 's' (wide string version).
		* @param	s	The string.
		* @param	c	The character to be counted.
		* @return	The number of 'c's in the string.
		*/
		static size_t	count(const std::wstring &s, wchar_t c);

		/**
		* @brief	Splits the string subject to given delimiter character.
		* 			split("a:b",':') --> "a","b"
		* 			split("a:",':') --> "a",""
		* 			split(":",':') --> "",""
		* 			split(":b",':') --> "","b"
		* 			split("a::b",':') --> "a","","b".
		*
		* @param	s		 	The string to process.
		* @param	delim	 	The delimiter.
		* @param [out]	elems	The splitted string parts.
		* @return	Number of splitted parts.
		*/
		static size_t split(const std::string &s, char delim, std::vector<std::string> &elems);

		/**
		* @brief	Splits the string subject to given delimiter character (wide string version).
		* 			split("a:b",':') --> "a","b"
		* 			split("a:",':') --> "a",""
		* 			split(":",':') --> "",""
		* 			split(":b",':') --> "","b"
		* 			split("a::b",':') --> "a","","b"
		* @param	s			 	The string to process.
		* @param	delim		 	The delimiter.
		* @param [out]	elems		The splitted string parts.
		* @return	Number of splitted parts.
		*/
		static size_t split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems);

		/**
		* @brief	Splits the string subject to given delimiter character.
		* @param	s			 	The string to process.
		* @param	delim		 	The delimiter.
		* @return	A vector of splitted string parts.
		*/
		static std::vector<std::string> split(const std::string &s, char delim);

		/**
		* @brief	Splits the string subject to given delimiter character (wide string version).
		* @param	s			 	The string to process.
		* @param	delim		 	The delimiter.
		* @return	A vector of splitted string parts.
		*/
		static std::vector<std::wstring> split(const std::wstring &s, wchar_t delim);

		/**
		* @brief	Trims the leading and trailing whitespaces (" ","\t","\n","\r") from the given string.
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::string trim(const std::string& str);

		/**
		* @brief	Trims the leading and trailing whitespaces (" ","\t","\n","\r") from the given string (wide string version).
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::wstring trim(const std::wstring& str);

		/**
		* @brief	Trims the leading whitespaces (" ","\t","\n","\r") from the given string.
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::string trimLeading(const std::string& str);

		/**
		* @brief	Trims the leading whitespaces (" ","\t","\n","\r") from the given string (wide string version).
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::wstring trimLeading(const std::wstring& str);

		/**
		* @brief	Trims the trailing whitespaces (" ","\t","\n","\r") from the given string.
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::string trimTrailing(const std::string& str);

		/**
		* @brief	Trims the trailing whitespaces (" ","\t","\n","\r") from the given string (wide string version).
		* @param	str	The string to be trimmed.
		* @return	Trimmed string.
		*/
		static std::wstring trimTrailing(const std::wstring& str);

		/**
		* @brief	Converts a string to lower case with given locale string .
		* @param	str	The string.
		* @return	Lower case string.
		*/
		static std::string toLower(std::string str, const std::string& localeString = "");

		/**
		* @brief	Converts a string to lower case with turkish locale.
		* @param	str	The string.
		* @return	Lower case string.
		*/
		static std::string toLowerTurkish(std::string str);

		/**
		* @brief	Converts a string to lower case with given locale string (wide string version).
		* @param	str	The string.
		* @return	Lower case string.
		*/
		static std::wstring toLower(std::wstring str, const std::string& localeString = "");

		/**
		* @brief	Converts a string to lower case with turkish locale (wide string version).
		* @param	str	The string.
		* @return	Lower case string.
		*/
		static std::wstring toLowerTurkish(std::wstring str);

		/**
		* @brief	Converts a string to upper case with given locale string .
		* @param	str	The string.
		* @return	Upper case string.
		*/
		static std::string toUpper(std::string str, const std::string& localeString = "");

		/**
		* @brief	Converts a string to upper case with turkish locale.
		* @param	str	The string.
		* @return	Upper case string.
		*/
		static std::string toUpperTurkish(std::string str);

		/**
		* @brief	Converts a string to upper case with given locale string  (wide string version).
		* @param	str	The string.
		* @return	Upper case string.
		*/
		static std::wstring toUpper(std::wstring str, const std::string& localeString = "");

		/**
		* @brief	Converts a string to upper case with turkish locale (wide string version).
		* @param	str	The string.
		* @return	Upper case string.
		*/
		static std::wstring toUpperTurkish(std::wstring str);

		/**
		* @brief	Converts a wide string to narrow with turkish locale.
		* @param	wstr	The string.
		* @return	Turkish narrow string.
		*/
		static std::string toNarrowTurkish(const std::wstring& wstr);

		/**
		* @brief	Converts a wide string to narrow using given locale string.
		* @param	str	The string.
		* @return	Narrow string. If the locale could not be created returns an empty string.
		*/
		static std::string toNarrow(const std::wstring& wstr, const std::string& localeString);

		/**
		* @brief	Converts a narrow Turkish string to wide using turkish locale.
		* @param	str	The narrow turkish string.
		* @return	Wide string.
		*/
		static std::wstring toWideTurkish(const std::string& str);

		/**
		* @brief	Converts a narrow string to wide using the given localestring .
		* @param	str	The narrow string.
		* @return	Wide string. If the locale could not be created returns an empty string.
		*/
		static std::wstring toWide(const std::string& str, const std::string& localeString);



		//TODO: document 
		static std::wstring convertToHMSM(double seconds_);
		//TODO: document 
		static std::wstring convertToHMSM(unsigned int milliseconds);

		/**
		* @brief	Converts an ios streamable type to string.
		* @tparam	typename T	Type of the typename t.
		* @param	val	The value to stringize.
		* @return	The string represantation.
		*/
		template<typename T> static std::string str(T val)
		{
			std::stringstream ss;
			ss << val;
			return ss.str();
		}

		/**
		* @brief	Converts an ios streamable type to wide string.
		* @tparam	typename T	Type of the typename t.
		* @param	val	The value to stringize.
		* @return	The wide string represantation.
		*/
		template<typename T> static std::wstring wstr(T val)
		{
			std::wstringstream ss;
			ss << val;
			return ss.str();
		}


		/**
		* @brief Convert a wide  string to a type
		* @tparam typename T type of the return value
		* @param wstr the string to get the value of
		* @return the value
		*/
		template<typename T> static T wstrToVal(std::wstring& wstr)
		{
			std::wstringstream ss;
			ss << wstr;
			T val;
			ss >> val;
			return val;
		}

		/**
		* @brief Convert a string to a type
		* @tparam typename T type of the return value
		* @param wstr the string to get the value of
		* @return the value
		*/
		template<typename T> static T strToVal(std::string& str)
		{
			std::stringstream ss;
			ss << str;
			T val;
			ss >> val;
			return val;
		}


		// UTF8 functions
		// ToDo: Needs a proper testing in different platforms..
		static std::string	wideToUTF8(std::wstring const &strWide);
		static std::wstring utf8ToWide(const std::string& strUTF8);

	};

} // namespace TACore 

#endif

#include "core/StringUtil.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace TACore
{

	// Codepage 1254, Turkish characters
#define SMALL_I		((char)253)
#define CAP_DOT_I	((char)221)


	// Turkish locale 
	static bool turkishLocale(std::locale& loc)
	{
		static std::locale turkishLocale;
		static bool inited = false;
		static bool hasTurkishLocale = false;

		if (!inited)
		{
			try
			{
#ifdef _MSC_VER
				turkishLocale = std::locale(StringUtil::getTurkishLocaleName().c_str());
#else
				turkishLocale = std::locale(StringUtil::getTurkishLocaleName().c_str());
#endif
				hasTurkishLocale = true;
			}
			catch (const std::runtime_error &err)
			{
				std::cerr << "Could not create turkish locale ( " << err.what() << "). Falling back to the default locale. " << std::endl;
				turkishLocale = std::locale("");
				hasTurkishLocale = false;
			}
			inited = true;
		}
		loc = turkishLocale;
		return hasTurkishLocale;
	}

	std::string StringUtil::getTurkishLocaleName()
	{
#ifdef _MSC_VER
		return("turkish");
#else
		return("tr_TR.utf8");
#endif
	}

	size_t StringUtil::count(const std::string &s, char c)
	{
		size_t cnt = 0;
		for (std::string::size_type i = 0; i<s.size(); ++i) if (s.data()[i] == c) ++cnt;
		return cnt;
	}

	size_t StringUtil::count(const std::wstring &s, wchar_t c)
	{
		size_t cnt = 0;
		for (std::string::size_type i = 0; i<s.size(); ++i) if (s.data()[i] == c) ++cnt;
		return cnt;
	}

	// Similar to: http://stackoverflow.com/questions/236129/c-how-to-split-a-string
	size_t StringUtil::split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
		if (s.empty()) return 0;

		std::stringstream ss(s);
		std::string item;
		size_t count = 0;
		while (std::getline(ss, item, delim))
		{
			elems.push_back(item);
			count++;
		}

		// split("a:",':') --> "a",""
		if (*s.rbegin() == delim)
		{
			elems.push_back("");
			count++;
		}
		return count;
	}

	size_t StringUtil::split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems)
	{
		if (s.empty()) return 0;
		std::wstringstream ss(s);
		std::wstring item;
		size_t count = 0;
		while (std::getline(ss, item, delim))
		{
			elems.push_back(item);
			count++;
		}

		// split(L"a:",L':') --> L"a",L""
		if (*s.rbegin() == delim)
		{
			elems.push_back(L"");
			count++;
		}
		return count;
	}

	std::vector<std::string> StringUtil::split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	std::vector<std::wstring> StringUtil::split(const std::wstring &s, wchar_t delim)
	{
		std::vector<std::wstring> elems;
		split(s, delim, elems);
		return elems;
	}

	// http://codereflect.com/2007/01/31/how-to-trim-leading-or-trailing-spaces-of-string-in-c/
	// Trim Both leading and trailing spaces
	std::string StringUtil::trim(const std::string& str)
	{
		size_t startpos = str.find_first_not_of(" \t\n\r"); // Find the first character position after excluding leading blank spaces
		size_t endpos = str.find_last_not_of(" \t\n\r"); // Find the first character position from reverse af

		// if all spaces or empty return an empty string
		if ((std::string::npos == startpos) || (std::string::npos == endpos))
			return "";
		else
			return str.substr(startpos, endpos - startpos + 1);
	}

	std::wstring StringUtil::trim(const std::wstring& str)
	{
		size_t startpos = str.find_first_not_of(L" \t\n\r"); // Find the first character position after excluding leading blank spaces
		size_t endpos = str.find_last_not_of(L" \t\n\r"); // Find the first character position from reverse af

		// if all spaces or empty return an empty string
		if ((std::wstring::npos == startpos) || (std::wstring::npos == endpos))
			return L"";
		else
			return str.substr(startpos, endpos - startpos + 1);
	}

	// Code for  Trim Leading Spaces only
	std::string StringUtil::trimLeading(const std::string& str)
	{
		size_t startpos = str.find_first_not_of(" \t\n\r"); // Find the first character position after excluding leading blank spaces
		if (std::string::npos != startpos)
			return str.substr(startpos);
		else
			return "";
	}

	// Code for Trim trailing Spaces only
	std::string StringUtil::trimTrailing(const std::string& str)
	{
		size_t endpos = str.find_last_not_of(" \t\n\r"); // Find the first character position from reverse af
		if (std::string::npos != endpos)
			return str.substr(0, endpos + 1);
		else
			return "";
	}

	// Code for  Trim Leading Spaces only
	std::wstring StringUtil::trimLeading(const std::wstring& str)
	{
		size_t startpos = str.find_first_not_of(L" \t\n\r"); // Find the first character position after excluding leading blank spaces
		if (std::wstring::npos != startpos)
			return str.substr(startpos);
		else
			return L"";
	}

	// Code for Trim trailing Spaces only
	std::wstring StringUtil::trimTrailing(const std::wstring& str)
	{
		size_t endpos = str.find_last_not_of(L" \t\n\r"); // Find the first character position from reverse af
		if (std::wstring::npos != endpos)
			return str.substr(0, endpos + 1);
		else
			return L"";
	}

	std::string StringUtil::toLower(std::string str, const std::string& localeString)
	{
		if (str.empty()) return "";
		std::locale loc;
		try
		{
			loc = std::locale(localeString.c_str());
		}
		catch (...) // catch (const std::runtime_error &err)
		{
			return "";
		}
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
			*it = std::use_facet<std::ctype<char> >(loc).tolower(*it);
		return str;
	}

	std::string StringUtil::toLowerTurkish(std::string str)
	{
		if (str.empty()) return "";
		std::locale loc;
		if (!turkishLocale(loc)) return "";
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			if (*it == 'I') *it = SMALL_I;
			else if (*it == CAP_DOT_I) *it = 'i';
			else *it = std::use_facet<std::ctype<char> >(loc).tolower(*it);
		}
		return str;
	}

	std::string StringUtil::toUpper(std::string str, const std::string& localeString)
	{
		if (str.empty()) return "";
		std::locale loc;
		try
		{
			loc = std::locale(localeString.c_str());
		}
		catch (...)
		{
			return "";
		}
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
			*it = std::use_facet<std::ctype<char> >(loc).toupper(*it);
		return str;
	}

	std::string StringUtil::toUpperTurkish(std::string str)
	{
		if (str.empty()) return "";
		std::locale loc;
		if (!turkishLocale(loc)) return "";
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			if (*it == SMALL_I) *it = 'I';
			else if (*it == 'i') *it = CAP_DOT_I;
			else *it = std::use_facet<std::ctype<char> >(loc).toupper(*it);
		}
		return str;
	}

	std::wstring StringUtil::toLower(std::wstring str, const std::string& localeString)
	{
		if (str.empty()) return L"";
		std::locale loc;
		try
		{
			loc = std::locale(localeString.c_str());
		}
		catch (...)
		{
			return L"";
		}
		for (std::wstring::iterator it = str.begin(); it != str.end(); ++it)
			*it = std::use_facet<std::ctype<wchar_t> >(loc).tolower(*it);
		return str;
	}

	std::wstring StringUtil::toLowerTurkish(std::wstring str)
	{
		if (str.empty()) return L"";
		std::locale loc;
		if (!turkishLocale(loc)) return L"";
		for (std::wstring::iterator it = str.begin(); it != str.end(); ++it)
		{
			if (*it == L'I')
				*it = 0x0131; //L'ý';
			else if (*it == 0x0130) //L'Ý'
				*it = L'i';
			else
				*it = std::use_facet<std::ctype<wchar_t> >(loc).tolower(*it);
		}
		return str;
	}


	std::wstring StringUtil::toUpper(std::wstring str, const std::string& localeString)
	{
		if (str.empty()) return L"";
		std::locale loc;
		try
		{
			loc = std::locale(localeString.c_str());
		}
		catch (...)
		{
			return L"";
		}
		for (std::wstring::iterator it = str.begin(); it != str.end(); ++it)
			*it = std::use_facet<std::ctype<wchar_t> >(loc).toupper(*it);
		return str;
	}

	std::wstring StringUtil::toUpperTurkish(std::wstring str)
	{
		if (str.empty()) return L"";
		std::locale loc;
		if (!turkishLocale(loc)) return L"";
		for (std::wstring::iterator it = str.begin(); it != str.end(); ++it)
		{
			if (*it == 0x0131) *it = L'I'; // L'ý'
			else if (*it == L'i') *it = 0x0130; //L'Ý';
			else *it = std::use_facet<std::ctype<wchar_t> >(loc).toupper(*it);
		}
		return str;
	}


	//Look at: http://stackoverflow.com/questions/4804298/c-how-to-convert-wstring-into-string
	std::string StringUtil::toNarrowTurkish(const std::wstring& wstr)
	{
		if (wstr.empty()) return "";
		wchar_t const* from = wstr.c_str();
		std::size_t const len = wstr.size();
		std::locale loc;
		if (!turkishLocale(loc)) return "";
		if (len<256)  // Faster path
		{
			char buffer[256];
			std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', buffer);
			return std::string(&buffer[0], &buffer[len]);
		}
		else
		{
			std::vector<char> buffer(len + 1);
			std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
			return std::string(&buffer[0], &buffer[len]);
		}
	}

	std::string StringUtil::toNarrow(const std::wstring& wstr, const std::string& localeString)
	{
		if (wstr.empty()) return "";
		std::locale loc;
		try
		{
			if (localeString.empty())
				loc = std::locale();
			else
				loc = std::locale(localeString.c_str());
		}
		catch (...)
		{
			return "";
		}

		wchar_t const* from = wstr.c_str();
		std::size_t const len = wstr.size();
		if (len<256) // Faster path
		{
			char buffer[256];
			std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', buffer);
			return std::string(&buffer[0], &buffer[len]);
		}
		else
		{
			std::vector<char> buffer(len + 1);
			std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
			return std::string(&buffer[0], &buffer[len]);
		}
	}


	// Look at: http://stackoverflow.com/questions/4804298/c-how-to-convert-wstring-into-string
	std::wstring StringUtil::toWide(const std::string& str, const std::string& localeString)
	{
		if (str.empty()) return L"";
		std::locale loc;
		try
		{
			if (localeString.empty())
				loc = std::locale();
			else
				loc = std::locale(localeString.c_str());
		}
		catch (...)
		{
			return L"";
		}

		char const* from = str.c_str();
		std::size_t const len = str.size();
		if (len<256) // Faster path
		{
			wchar_t buffer[256];
			std::use_facet<std::ctype<wchar_t> >(loc).widen(from, from + len, buffer);
			return std::wstring(&buffer[0], &buffer[len]);
		}
		else
		{
			std::vector<wchar_t> buffer(len + 1);
			std::use_facet<std::ctype<wchar_t> >(loc).widen(from, from + len, &buffer[0]);
			return std::wstring(&buffer[0], &buffer[len]);
		}
	}


	std::wstring StringUtil::toWideTurkish(const std::string& str)
	{
		if (str.empty()) return L"";
		char const* from = str.c_str();
		std::size_t const len = str.size();
		std::locale loc;
		if (!turkishLocale(loc)) return L"";
		if (len<256) // Faster path
		{
			wchar_t buffer[256];
			std::use_facet<std::ctype<wchar_t> >(loc).widen(from, from + len, buffer);
			return std::wstring(&buffer[0], &buffer[len]);
		}
		else
		{
			std::vector<wchar_t> buffer(len + 1);
			std::use_facet<std::ctype<wchar_t> >(loc).widen(from, from + len, &buffer[0]);
			return std::wstring(&buffer[0], &buffer[len]);
		}
	}

	// Be careful using UTF8<-->wide conversion functions. 
	// Although toUTF8 and toWideFromUTF8 seem to work properly with Turkish, it may not be ok for other languages ! Requires more testing. 
	std::string StringUtil::wideToUTF8(const std::wstring& src)
	{
		std::string dest;
		dest.clear();
		if (src.empty()) return dest;

		dest.reserve(src.size());
		for (size_t i = 0; i < src.size(); i++){
			wchar_t w = src[i];
			if (w <= 0x7f)
				dest.push_back((char)w);
			else if (w <= 0x7ff){
				dest.push_back(0xc0 | ((w >> 6) & 0x1f));
				dest.push_back(0x80 | (w & 0x3f));
			}
			else if (w <= 0xffff){
				dest.push_back(0xe0 | ((w >> 12) & 0x0f));
				dest.push_back(0x80 | ((w >> 6) & 0x3f));
				dest.push_back(0x80 | (w & 0x3f));
			}
			else
				dest.push_back('?');
		}
		return dest;
	}

	std::wstring
		StringUtil::utf8ToWide(const std::string& src)
	{
		std::wstring dest;
		dest.clear();
		if (src.empty()) return dest;

		dest.reserve(src.size());
		for (size_t i = 0; i < src.size(); i++)
		{
			unsigned char c = src[i];
			if (c <= 0x7f)
				dest.push_back((wchar_t)c);
			else if ((c & 0xc0) == 0xc0)
			{
				wchar_t w = 0;
				w = (c & 0x1f) << 6;

				++i;
				if (i == src.size()) break;
				c = src[i];
				w |= (c & 0x3f);
				dest.push_back(w);
			}
			else if ((c & 0xe0) == 0xe0)
			{
				wchar_t w = 0;
				w = (c & 0x0f) << 12;

				++i;
				if (i == src.size()) break;
				c = src[i];
				w |= (c & 0x3f);

				++i;
				if (i == src.size()) break;
				c = src[i];
				w |= (c & 0x3f);
				dest.push_back(w);
			}
			else
				dest.push_back(L'?');
		}
		return dest;
	}


	std::wstring StringUtil::convertToHMSM(double seconds_)
	{
		// return convertToHMSM(unsigned int(seconds*1000));
		//the above call doesn't work in 32 bit linux so code is duplicated from the uint version.
		unsigned int milliseconds = static_cast<unsigned int>(seconds_ * 1000.0);
		unsigned int milli = milliseconds % 1000;	milliseconds = milliseconds / 1000;
		unsigned int seconds = milliseconds % 60;	milliseconds = milliseconds / 60;
		unsigned int minutes = milliseconds % 60;	milliseconds = milliseconds / 60;
		unsigned int hours = milliseconds % 60;
		wchar_t bfr[32];
		swprintf(bfr, 31, L"%02d:%02d:%02d.%03d", hours, minutes, seconds, milli);
		return std::wstring(bfr);
	}

	std::wstring StringUtil::convertToHMSM(unsigned int milliseconds)
	{
		unsigned int milli = milliseconds % 1000;	milliseconds = milliseconds / 1000;
		unsigned int seconds = milliseconds % 60;	milliseconds = milliseconds / 60;
		unsigned int minutes = milliseconds % 60;	milliseconds = milliseconds / 60;
		unsigned int hours = milliseconds % 60;
		wchar_t bfr[32];
		swprintf(bfr, 31, L"%02d:%02d:%02d.%03d", hours, minutes, seconds, milli);
		return std::wstring(bfr);
	}





} // namespace KCore 

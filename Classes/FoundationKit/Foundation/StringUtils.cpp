/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.

  losemymind.libo@gmail.com

****************************************************************************/
#include <locale>
#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <cctype>
#include <sstream>
//#include <codecvt>
#include "StringUtils.h"
#include "FoundationKit/external/ConvertUTF/ConvertUTF.h"

NS_FK_BEGIN

std::string StringUtils::tolower( const std::string& str )
{
	std::string ret;
	for (auto ch : str)
	{
		if (ch >= 'A' && ch <= 'Z')
			ret += ch | 32;
		else
			ret += ch;
	}
	return ret;
}

std::string StringUtils::toupper( const std::string& str )
{
	std::string ret;
	for (auto ch : str)
	{
		if (ch >= 'a' && ch <= 'z')
			ret += ch ^ 32;
		else
			ret += ch;
	}
	return ret;
}

std::string StringUtils::format( const char* format, ... )
{
	const static unsigned int MAX_STRING_LENGTH = 64;
	va_list arglist;
	int size = MAX_STRING_LENGTH;
	std::vector<char> dynamicBuffer(MAX_STRING_LENGTH);
	char* str = &dynamicBuffer[0];
	for (;;)
	{
        va_start(arglist, format);
        // Pass one less than size to leave room for NULL terminator
        int needed = vsnprintf(str, size - 1, format, arglist);
        va_end(arglist);

		// NOTE: Some platforms return -1 when vsnprintf runs out of room, while others return
		// the number of characters actually needed to fill the buffer.
		if (needed >= 0 && needed < size)
		{
			// Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
			str[needed] = '\0';
			break;
		}
		size = needed > 0 ? (needed + 1) : (size * 2);
		dynamicBuffer.resize(size);
		str = &dynamicBuffer[0];
	}
	return str;
}

std::string & StringUtils::ltrim( std::string &s )
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string & StringUtils::rtrim( std::string &s )
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::string & StringUtils::trim( std::string &s )
{
	return ltrim(rtrim(s));
}

std::vector<std::string> & StringUtils::split( const std::string &s, char delim, std::vector<std::string> &elems )
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> StringUtils::split( const std::string &s, char delim )
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string StringUtils::join(std::string delim, std::vector<std::string> values)
{
	std::string ret;
	size_t index = 0;
	size_t valuesSize = values.size();
	for (auto& str : values)
	{
		++index;
		ret.append(str);
		if (index < valuesSize)
		{
			ret.append(delim);
		}
	}
	return ret;
}

bool StringUtils::isNumber( const std::string& val )
{
	std::stringstream str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}


// Other impl see:http://blog.poxiao.me/p/unicode-character-encoding-conversion-in-cpp11/

std::string StringUtils::wstring2UTF8string(const std::wstring &input)
{
    std::locale old_loc = std::locale::global(std::locale(""));
    const wchar_t* src_wstr = input.c_str();
    size_t buffer_size = input.size() * 4 + 1;
    char* dst_str = new char[buffer_size];
    memset(dst_str, 0, buffer_size);
    wcstombs(dst_str, src_wstr, buffer_size);
    std::string result = dst_str;
    delete[]dst_str;
    std::locale::global(old_loc);
    return result;
}

std::wstring StringUtils::string2UTF8wstring(const std::string &input)
{
    std::locale old_loc = std::locale::global(std::locale(""));
    const char* src_str = input.c_str();
    const size_t buffer_size = input.size() + 1;
    wchar_t* dst_wstr = new wchar_t[buffer_size];
    wmemset(dst_wstr, 0, buffer_size);
    mbstowcs(dst_wstr, src_str, buffer_size);
    std::wstring result = dst_wstr;
    delete[]dst_wstr;
    std::locale::global(old_loc);
    return result;
}


bool StringUtils::UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16)
{
    if (utf8.empty())
    {
        outUtf16.clear();
        return true;
    }

    bool ret = false;

    const size_t utf16Bytes = (utf8.length() + 1) * sizeof(char16_t);
    char16_t* utf16 = (char16_t*)malloc(utf16Bytes);
    memset(utf16, 0, utf16Bytes);

    char* utf16ptr = reinterpret_cast<char*>(utf16);
    const UTF8* error = nullptr;

    if (llvm::ConvertUTF8toWide(2, utf8, utf16ptr, error))
    {
        outUtf16 = utf16;
        ret = true;
    }
    free(utf16);
    return ret;
}

bool StringUtils::UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8)
{
    if (utf16.empty())
    {
        outUtf8.clear();
        return true;
    }

    return llvm::convertUTF16ToUTF8String(utf16, outUtf8);
}

/*
std::u16string StringUtils::UTF8ToUTF16(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.from_bytes(s);
}

std::u32string StringUtils::UTF8ToUTF32(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

std::string StringUtils::UTF16ToUTF8(const std::u16string &s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(s);
}

std::u32string StringUtils::UTF16ToUTF32(const std::u16string &s)
{
    const char16_t *pData = s.c_str();
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    return conv.from_bytes(reinterpret_cast<const char*>(pData), reinterpret_cast<const char*>(pData + s.length()));
}

std::string StringUtils::UTF32ToUTF8(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}


std::u16string StringUtils::UTF32ToUTF16(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    std::string bytes = conv.to_bytes(s);
    return std::u16string(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length() / sizeof(char16_t));
}
*/
NS_FK_END

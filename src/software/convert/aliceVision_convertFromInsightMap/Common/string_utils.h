#ifndef COMMON_SPLIT_H
#define COMMON_SPLIT_H

#include <vector>
#include <string>
#include <sstream>
#include "common_global.h"
#include <algorithm>

//#include <QString>



INSIGHT_NAME_SPACE
/**
* Split an input string with a delimiter and fill a string vector
*/
static bool split(const std::string &src, const std::string& delim, std::vector<std::string>& vec_value)
{
	bool bDelimiterExist = false;
	if (!delim.empty())
	{
		vec_value.clear();
		std::string::size_type start = 0;
		std::string::size_type end = std::string::npos - 1;
		while (end != std::string::npos)
		{
			end = src.find(delim, start);
			vec_value.push_back(src.substr(start, end - start));
			start = end + delim.size();
		}
		if (vec_value.size() >= 2)
			bDelimiterExist = true;
	}
	return bDelimiterExist;
}

static bool split(const std::string &s, const char delim, std::vector<std::string> &tokens) {
	tokens.clear();
	std::stringstream ss(s);
	std::string token;
	while (std::getline(ss, token, delim)) {
		tokens.push_back(token);
	}
	return tokens.size() >= 2;
}

static std::string& trim(std::string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

static std::string &toLowerStr(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

// #define tos(str) \
// 	(std::string((str).toLocal8Bit().constData()))
// 
// #define toqs(str) \
// 	QString::fromLocal8Bit((str).c_str(
#if 0
inline std::string tos(const QString &s){
	return s.toLocal8Bit().constData();
}

inline QString toqs(const std::string &s){
	return QString::fromLocal8Bit(s.c_str());
}
#endif
void StringAppendV(std::string* dst, const char* format, va_list ap);


std::string StringPrintf(const char* format, ...);

INSIGHT_NAME_SPACE_END
#endif // COMMON_SPLIT_H


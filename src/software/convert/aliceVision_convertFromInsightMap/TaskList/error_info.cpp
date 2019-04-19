#include "error_info.h"
#include <glog/logging.h>

#include <hash_map>
#include <stlplus3/filesystemSimplified/file_system.hpp>

#include <fstream>
#include "common/string_utils.h"


INSIGHT_NAME_SPACE

int g_last_error = NoError;

std::hash_map<int, std::string> g_error_map;

TASKLIST_EXPORT int insight::LastError()
{
	int last_error = g_last_error;
	g_last_error = NoError;
	return  last_error;
}

TASKLIST_EXPORT const char * ErrorString(int error_code)
{
	CHECK(g_error_map.find(error_code) != g_error_map.end());
	return g_error_map[error_code].c_str();
}

TASKLIST_EXPORT void SetLastError(int error_code)
{
	g_last_error = error_code;
}

TASKLIST_EXPORT int InitErrorCode(std::string &error_code_file)
{
	g_error_map.clear();
	if (!stlplus::file_exists(error_code_file)) return EXIT_FAILURE;
	std::ifstream ifs(error_code_file);
	std::string s;
	int code;
	while (std::getline(ifs, s)){
		std::vector<std::string> tokens;
		split(s, ";", tokens);
		CHECK(tokens.size() == 2);
		std::stringstream ss;
		ss.str(tokens[0]);
		ss >> code;
		g_error_map[code] = tokens[1];
	}
	return EXIT_SUCCESS;
}

INSIGHT_NAME_SPACE_END


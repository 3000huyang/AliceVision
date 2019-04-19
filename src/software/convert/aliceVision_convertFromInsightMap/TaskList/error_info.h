#ifndef error_info_h__
#define error_info_h__

#include "tasklist_global.h"

#include <string>

INSIGHT_NAME_SPACE

enum ErrorCode{
	NoError = 0,

};
/*
返回最后一次错误代码，并清空错误，lasterror变成NoError
*/
TASKLIST_EXPORT int LastError();
TASKLIST_EXPORT const char *ErrorString(int error_code);
TASKLIST_EXPORT void SetLastError(int error_code);

TASKLIST_EXPORT int InitErrorCode(std::string &error_code_file);

INSIGHT_NAME_SPACE_END
#endif // error_info_h__

#ifndef	WXJ_COMMON_WXJMACROS_H
#define WXJ_COMMON_WXJMACROS_H

#ifdef W_OS_WIN
#define NOMINMAX
#include <Windows.h>
#include <ShlObj.h>
#endif

namespace Wxj
{
	enum CaseSensitivity
	{
		CS_Insensitive = 0,//ºöÂÔ
		CS_Sensitive = 1//²»ºöÂÔ
	};
}


#define WNOCOPY_CLASS(class_name) \
	private:\
		class_name(const class_name &);\
		class_name &operator=(const class_name &);


#endif
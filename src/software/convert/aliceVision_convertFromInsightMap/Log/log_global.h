#ifndef LOG_GLOBAL_H
#define LOG_GLOBAL_H

//#include <QtCore/qglobal.h>

#ifdef LOG_LIB

# define LOG_EXPORT __declspec(dllexport)
#else
# define LOG_EXPORT  __declspec(dllimport)
#endif


#ifndef INSIGHT_NAME_SPACE
#define INSIGHT_NAME_SPACE namespace insight{
#define INSIGHT_NAME_SPACE_END }
#endif

#endif // LOG_GLOBAL_H

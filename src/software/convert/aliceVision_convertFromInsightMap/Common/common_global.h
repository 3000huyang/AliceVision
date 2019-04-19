#ifndef COMMON_GLOBAL_H
#define COMMON_GLOBAL_H

//#include <QtCore/qglobal.h>

#define COMMON_EXPORT

// #ifdef COMMON_LIB
// # define COMMON_EXPORT __declspec(dllexport)
// #else
// # define COMMON_EXPORT __declspec(dllimport)
// #endif

#ifndef INSIGHT_NAME_SPACE
#define INSIGHT_NAME_SPACE namespace insight{
#define INSIGHT_NAME_SPACE_END }
#endif

#endif // COMMON_GLOBAL_H

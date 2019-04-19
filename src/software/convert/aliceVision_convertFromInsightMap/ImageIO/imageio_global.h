#ifndef IMAGEIO_GLOBAL_H
#define IMAGEIO_GLOBAL_H

//#include <QtCore/qglobal.h>

#ifdef IMAGEIO_LIB
# define IMAGEIO_EXPORT __declspec(dllexport)
#else
# define IMAGEIO_EXPORT __declspec(dllimport)
#endif

#ifndef INSIGHT_NAME_SPACE
#define INSIGHT_NAME_SPACE namespace insight{
#define INSIGHT_NAME_SPACE_END }
#endif
#include <cstdint>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
#endif // IMAGEIO_GLOBAL_H

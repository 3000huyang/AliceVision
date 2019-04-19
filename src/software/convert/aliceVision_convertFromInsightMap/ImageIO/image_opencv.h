#ifndef image_opencv_h__
#define image_opencv_h__

#include "imageio_global.h"
#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>

INSIGHT_NAME_SPACE

IMAGEIO_EXPORT bool readJPGImage(std::string &jpg_file, std::vector<uint8_t> &buffer, int &w, int &h, int &channels, int &depth);

INSIGHT_NAME_SPACE_END


#endif // image_opencv_h__

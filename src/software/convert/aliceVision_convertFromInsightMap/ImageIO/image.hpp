
#ifndef INSIGHT_IMAGE_HPP
#define INSIGHT_IMAGE_HPP

// Get rid of the specific MSVC compiler warnings.
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
# define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <vector>

#include "common/numeric.h"

#include "image_container.hpp"
#include "pixel_types.hpp"
#include "image_converter.hpp"
#include "image_drawing.hpp"
#include "image_concat.hpp"
#include "sample.hpp"

#include "image_convolution_base.hpp"
#include "image_convolution.hpp"
#include "image_filtering.hpp"
#include "image_resampling.hpp"
#include "image_diffusion.hpp"

#endif /* INSIGHT_IMAGE_HPP */

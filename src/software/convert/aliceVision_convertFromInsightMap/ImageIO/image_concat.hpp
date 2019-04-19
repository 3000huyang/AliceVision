#ifndef INSIGHT_IMAGE_IMAGE_CONCAT_H_
#define INSIGHT_IMAGE_IMAGE_CONCAT_H_

#include "imageio_global.h"

#include "image_container.hpp"

INSIGHT_NAME_SPACE

/// Horizontal concatenation of images
template < class Image >
void ConcatH(const Image & imageA, const Image & imageB, Image & Out)
{
  // Compute new dimensions // |imgA|+|imgB|
  int ww = imageA.Width() + imageB.Width();
  Out.resize(ww, std::max(imageA.Height(), imageB.Height()));

  // Copy the first image |imgA|...|
  Out.block(0,0, imageA.Height(), imageA.Width()) = imageA.GetMat();
  // Copy the second image |imgA|imgB|
  Out.block(0, imageA.Width(), imageB.Height(), imageB.Width()) = imageB.GetMat();
}

/// Vertical concatenation of images
template < class Image >
void ConcatV(const Image & imageA, const Image & imageB, Image & Out)
{
  // Compute new dimensions
  // |imgA|
  // |imgB|
  int hh = imageA.Height() + imageB.Height();
  Out.resize(max(imageA.Width(), imageB.Width()), hh);

  // Copy the first image
  // |imgA|
  // |....|
  Out.block(0,0, imageA.Height(), imageA.Width()) = imageA.GetMat();
  // Copy the second image
  // |imgA|
  // |imgB|
  Out.block(imageA.Height(), 0, imageB.Height(), imageB.Width()) = imageB.GetMat();
}

INSIGHT_NAME_SPACE_END

#endif 
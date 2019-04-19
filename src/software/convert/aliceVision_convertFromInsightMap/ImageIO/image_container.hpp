
#ifndef INSIGHT_IMAGE_CONTAINER_HPP
#define INSIGHT_IMAGE_CONTAINER_HPP

#include "common/numeric.h"

INSIGHT_NAME_SPACE

template <typename T>
class Image : public Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
{

public:
  typedef T Tpixel;	//-- Pixel data type
  typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Base;

  //------------------------------
  //-- Image construction method
  inline Image() {};

  inline Image(int width, int height, bool fInit=true, const T val = T())
  {
    Base::resize(height, width);
    if (fInit) Base::fill(val);
  };

  inline Image(const Base& I)  { (*this) = I; }
  inline Image& operator=(const Base& I) { Base::operator=(I);  return *this; }
  virtual inline ~Image() {};
  //-- Image construction method
  //------------------------------

  //-- Resize overhead
  inline void resize(int width, int height, bool fInit=true, const T val = T(0))
  {
    Base::resize(height, width);
    if (fInit) Base::fill(val);
  }

  //------------------------------
  //-- accessors/getters methods
  /// Retrieve the width of the image
  inline int Width()  const { return static_cast<int>(Base::cols()); }
  /// Retrieve the height of the image
  inline int Height() const { return static_cast<int>(Base::rows()); }
  /// Return the depth in byte of the pixel (unsigned char will return 1)
  inline int Depth() const  { return sizeof(Tpixel); }

  /// random pixel access
  inline const T& operator()(int y, int x) const { return Base::operator()(y,x); }
  /// random pixel access
  inline T& operator()(int y, int x) { return Base::operator()(y,x); }
  // sub-pixel access
  //T operator()(double x, double y, Tinterpolation eInterpMode = Image::INTER_BILINEAR) const;

  inline const Base& GetMat() const { return (*this); }

  //-- accessors/getters methods
  //------------------------------

  /// Tell if a point is inside the image.
  inline bool Contains(int y, int x) const  {
    return 0 <= x && x < Base::cols()
      && 0 <= y && y < Base::rows();
  }

protected :
  //-- Image data are stored by inheritance of a matrix
};

INSIGHT_NAME_SPACE_END

#endif // INSIGHT_IMAGE_CONTAINER_HPP

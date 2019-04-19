#ifndef WXJ_COMMON_TRAITS_H
#define WXJ_COMMON_TRAITS_H
/**
 * @brief ºÚµ•¿‡–Õtraits£¨int£¨float£¨double
 **/
template <typename T>
struct WIFDTraits
{
	enum {IS_VALID = 0};
};

template<>
struct WIFDTraits<int>
{
	enum {IS_VALID = 1};
};
template<>
struct WIFDTraits<double>
{
	enum {IS_VALID = 1};
};
template<>
struct WIFDTraits<float>
{
	enum {IS_VALID = 1};
};

#endif
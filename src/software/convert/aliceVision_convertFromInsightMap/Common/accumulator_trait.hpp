// Copyright (c) 2014 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INSIGHT_COMMON_ACCUMULATOR_TRAIT_HPP_
#define INSIGHT_COMMON_ACCUMULATOR_TRAIT_HPP_

/// Accumulator trait to perform safe summation over a specified type
INSIGHT_NAME_SPACE

template<typename T>
struct Accumulator { typedef T Type; };
template<>
struct Accumulator<unsigned char>  { typedef float Type; };
template<>
struct Accumulator<unsigned short> { typedef float Type; };
template<>
struct Accumulator<unsigned int> { typedef float Type; };
template<>
struct Accumulator<char>   { typedef float Type; };
template<>
struct Accumulator<short>  { typedef float Type; };
template<>
struct Accumulator<int> { typedef float Type; };
template<>
struct Accumulator<bool>  { typedef unsigned int Type; };

INSIGHT_NAME_SPACE_END

#endif //INSIGHT_COMMON_ACCUMULATOR_TRAIT_HPP_
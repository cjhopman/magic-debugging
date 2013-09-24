#ifndef MAGIC_MATH_H
#define MAGIC_MATH_H

#include_next <stl/_cmath.h>

#if defined(OS_ANDROID) || defined(ANDROID)
#if defined(__cplusplus)
using std::isfinite;
using std::isinf;
using std::isnan;
using std::signbit;
#endif
#endif

#endif

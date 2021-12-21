#ifndef __COMMON_H
#define __COMMON_H

#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi = 3.141592653589793238462643383279502884L;
#endif

struct Point {
  int x, y;
};

#endif
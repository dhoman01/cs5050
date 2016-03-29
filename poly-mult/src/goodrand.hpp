#ifndef GOOD_RAND_HPP
#define GOOD_RAND_HPP

#include <random>

namespace RAND {
int getRand(int l, int u) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<> d(l, u);
  return d(mt);
}

double getRand(double l, double u) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_real_distribution<> d(l, u);
  return d(mt);
}
}

#endif

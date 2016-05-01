#ifndef GOOD_RAND_HPP
#define GOOD_RAND_HPP

#include <random>

namespace RAND {

// Get a random integer with
// uniform distribution in the
// range l to u
int getRand(int l, int u) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<> d(l, u);
  return d(mt);
}

// Get a random double with
// uniform distribution in the
// range l to u
double getRand(double l, double u) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_real_distribution<> d(l, u);
  return d(mt);
}
}

#endif

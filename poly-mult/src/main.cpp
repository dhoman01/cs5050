#include "goodrand.hpp"
#include "poly.hpp"
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  const auto N_START = 32;
  const auto N_END = std::pow(2, 25);

  const auto P_COEF_LOW = -1.0;
  const auto P_COEF_HIGH = 1.0;
  const auto Q_COEF_LOW = -1.0;
  const auto Q_COEF_HIGH = 1.0;

  // Column names for output
  std::cout << "n,Naive (micro sec),Fast (micro sec)" << std::endl;

  for (int n = N_START; n < N_END; n *= 2) {
    // Change type to int if P_COEF
    // and Q_COEF are integers
    std::vector<double> P;
    std::vector<double> Q;

    // Generate problem set
    for (int i = 0; i < n; i++) {
      P.push_back(RAND::getRand(P_COEF_LOW, P_COEF_HIGH));
      Q.push_back(RAND::getRand(Q_COEF_LOW, Q_COEF_HIGH));
    }

    // Do naive polynomial multiplication
    auto naiveStart = std::chrono::steady_clock::now();
    auto PQ = POLY::naiveMult(P, Q);
    auto naiveEnd = std::chrono::steady_clock::now();

    // Do divide and conquer polynomial multiplication
    auto start = std::chrono::steady_clock::now();
    auto F = POLY::mult(P, Q);
    auto end = std::chrono::steady_clock::now();

    assert(POLY::equal(*PQ, F));

    // Print out results of problem set
    auto naive = std::chrono::duration_cast<std::chrono::microseconds>(
        naiveEnd - naiveStart);
    auto fast =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Write data out
    std::cout << n << "," << naive.count() << "," << fast.count() << std::endl;
  }

  return EXIT_SUCCESS;
}

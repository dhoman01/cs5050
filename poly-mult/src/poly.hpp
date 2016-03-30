#ifndef POLY_HPP
#define POLY_HPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

namespace POLY {

const double EPSILON = .00000001;

// Print a polynomial
template <typename T> std::string printPoly(T &P) {
  std::stringstream ss;
  for (int i = 0; i < P.size(); i++) {
    ss << P[i] << "x^" << i << (i == P.size() - 1 ? "" : " + ");
  }
  return ss.str();
}

// Check that two polynomials are equal.
// For doubles ensure they are close enough,
// as rounding will cause some descripency.
// Adjust EPSILON for precision
template <typename T> bool equal(T &P, T &Q) {
  for (int i = 0; i < P.size(); i++) {
    if (std::abs(P[i] - Q[i]) > EPSILON) {
      return false;
    }
  }

  return true;
}

// Sum the Coefficients of two polynomials
template <typename T> T sum(T &P, T &Q) {
  T S(P.size(), 0);

  for (int i = 0; i < P.size(); i++) {
    S[i] = P[i] + Q[i];
  }

  return S;
}

// Subtract the Coefficients of two polynomials
template <typename T> T sub(T &P, T &Q) {
  T D(P.size(), 0);

  for (int i = 0; i < P.size(); i++) {
    D[i] = P[i] - Q[i];
  }

  return D;
}

// Construct solution to 3x mult: E + F + G
template <typename T> T constructSolution(T &low, T &mid, T &high) {
  auto n = (low.size() % 2 == 0 ? low.size() : low.size() * 2);

  T sol(n * 2, 0);
  for (int i = 0; i < n; i++) {
    sol[i] += (i < low.size() ? low[i] : 0);
    sol[i + (n / 2)] += (i < mid.size() ? mid[i] : 0);
    sol[i + n] += (i < high.size() ? high[i] : 0);
  }

  return sol;
}

// Naive polynomial multiplication algorithm
template <typename T> std::unique_ptr<T> naiveMult(T &P, T &Q) {
  auto PQ = std::make_unique<T>(P.size() * 2 - 1, 0);
  for (int i = 0; i < P.size(); i++) {
    for (int j = 0; j < P.size(); j++) {
      (*PQ)[i + j] += P[i] * Q[j];
    }
  }

  return PQ;
}

// 3x mult, divide and conquer algorithm
// for polynomial multiplication
template <typename T> T mult(T &P, T &Q) {
  // Simple Problem
  if (P.size() == 1 && Q.size() == 1) {
    T PQ(1, 0);
    PQ[0] = P[0] * Q[0];
    return PQ;
  }

  // Midpoint
  int mid = P.size() / 2;

  // Problem decomposition
  T P_low(P.begin(), P.begin() + mid); // a
  T P_high(P.begin() + mid, P.end());  // b
  T Q_low(Q.begin(), Q.begin() + mid); // c
  T Q_high(Q.begin() + mid, Q.end());  // d

  /**********************************
    * 3x mult:                      *
    * sol for (a + b)(c + d)        *
    * E = (a * c)                   *
    * G = (c * d)                   *
    * F = (a + b) * (c + d) - E - G *
    * sol = E + F + G               *
   **********************************/
  auto sol_l = mult(P_low, Q_low);   // (a * c)
  auto sol_h = mult(P_high, Q_high); // (b * d)
  auto sum_P = sum(P_low, P_high);   // (a + b)
  auto sum_Q = sum(Q_low, Q_high);   // (c + d)
  auto sol_m = mult(sum_P, sum_Q);   // (a + b) * (c + d)

  sol_m = sub(sol_m, sol_l); // - (a * c)
  sol_m = sub(sol_m, sol_h); // - (b * d)

  return constructSolution(sol_l, sol_m, sol_h);
}
}

#endif

#ifndef POLY_HPP
#define POLY_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

namespace POLY {

const double EPSILON = .0000001;

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
template <typename T> bool equal(T &P, T &Q, int n) {
  for (int i = 0; i < n; i++) {
    if (std::abs(P[i] - Q[i]) > EPSILON) {
      return false;
    }
  }

  return true;
}

// Multiply the values of two arrays
template <typename T> T multiplyValues(T &P, T &Q) {
  T PQ(P.size());
  for (int i = 0; i < P.size(); i++) {
    PQ[i] = P[i] * Q[i];
  }

  return PQ;
}

// Multiply the Coefficients of a polynomial by a constant value
template <typename T, typename V> void multiplyValues(T &P, V value) {
  std::for_each(P.begin(), P.end(), [=](auto &n) { n *= value; });
}

// Divide teh Coefficients of a polynomial by a constant value
template <typename T, typename V> void divideValues(T &P, V value) {
  std::for_each(P.begin(), P.end(), [=](auto &n) { n /= value; });
}

// Invert the Coefficients of a polynomial
template <typename T> void invert(T &P) {
  std::for_each(P.begin(), P.end(), [](auto &n) { n = std::pow(n, -1); });
}

// Invert a copy of a polynomial
template <typename T> T copy_invert(T &P) {
  T P_inv(P);
  invert(P_inv);
  return P_inv;
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

template <typename T> T FFT(T &P, T &W, int n) {
  // Simplest problem
  if (n == 1)
    return P;

  // Problem decomposition
  T P_odd(n / 2);
  T P_even(n / 2);
  for (int i = 0; i < (n / 2); i++) {
    P_odd[i] = P[2 * i + 1];
    P_even[i] = P[2 * i];
  }

  // Generate correct omega values
  T W_2(n / 2);
  for (int j = 0; j < (n / 2); j++) {
    W_2[j] = W[2 * j] * W[2 * j];
  }

  // Compute sub-problems
  auto soleven = FFT(P_even, W_2, n / 2);
  auto solodd = FFT(P_odd, W_2, n / 2);

  // Construct solution
  T sol(n);
  for (int j = 0; j < (n / 2); j++) {
    sol[j] = soleven[j] + W[j] * solodd[j];
    sol[j + n / 2] = soleven[j] - W[j] * solodd[j];
  }

  return sol;
}

// Pad the higher order Coefficients of P with 0
template <typename T> void padHighOrder(T &P, int n) {
  for (int i = 0; i < n; i++)
    P.push_back(0);
}

// Perform polynomial multiplication using the FFT
template <typename T> T FFTmult(T &P, T &Q, T &W, int n) {
  // Pad P and Q with zeros
  padHighOrder(P, n);
  padHighOrder(Q, n);

  assert(P.size() == 2 * n);

  // Evaluate P and Q at nth roots
  // of unity
  auto P_x = FFT(P, W, 2 * n);
  auto Q_x = FFT(Q, W, 2 * n);
  auto PQ_x = multiplyValues(P_x, Q_x);

  // Calculate the nth root inverses
  T W_inv = copy_invert(W);

  // Find the Coefficients of PQ
  // via Interpolation with the
  // inverse of FFT
  auto PQ = FFT(PQ_x, W_inv, 2 * n);

  // Divide the above result by
  // 2*n to complete calculation of
  // the inverse of FFT
  divideValues(PQ, (2 * n));

  // Return the solution
  return PQ;
}
}

#endif

#ifndef POLY_HPP
#define POLY_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace POLY {

template <typename T> std::string printPoly(T &P) {
  std::stringstream ss;
  for (int i = 0; i < P.size(); i++) {
    ss << P[i] << "x^" << i << (i == P.size() - 1 ? "" : " + ");
  }
  return ss.str();
}

template <typename T> T sum(T &P, T &Q) {
  T S(P.size(), 0);

  for (int i = 0; i < P.size(); i++) {
    S[i] = P[i] + Q[i];
  }

  return S;
}

template <typename T> T sub(T &P, T &Q) {
  T D(P.size(), 0);

  for (int i = 0; i < P.size(); i++) {
    D[i] = P[i] - Q[i];
  }

  return D;
}

template <typename T> T constructSolution(T &low, T &mid, T &high) {
  // std::cout << "low: " << printPoly(low) << std::endl;
  // std::cout << "mid: " << printPoly(mid) << std::endl;
  // std::cout << "high: " << printPoly(high) << std::endl;
  auto n = (low.size() % 2 == 0 ? low.size() : low.size() * 2);
  // std::cout << "n: " << n << std::endl;
  T sol(n * 2, 0);
  for (int i = 0; i < n; i++) {
    sol[i] += (i < low.size() ? low[i] : 0);
    sol[i + (n / 2)] += (i < mid.size() ? mid[i] : 0);
    sol[i + n] += (i < high.size() ? high[i] : 0);
    // std::cout << "i=" << i << std::endl;
    // std::cout << "sol[" << i << "]=" << sol[i] << std::endl;
    // std::cout << "sol[" << i + (n / 2) << "]=" << sol[i + (n / 2)] <<
    // std::endl;
    // std::cout << "sol[" << i + n << "]=" << sol[i + n] << std::endl;
  }

  return sol;
}

template <typename T> std::unique_ptr<T> naiveMult(T &P, T &Q) {
  auto PQ = std::make_unique<T>(P.size() * 2 - 1, 0);
  for (int i = 0; i < P.size(); i++) {
    for (int j = 0; j < P.size(); j++) {
      (*PQ)[i + j] += P[i] * Q[j];
    }
  }

  return PQ;
}

template <typename T> T mult(T &P, T &Q) {
  // std::cout << "P: " << printPoly(P) << "   ";
  // std::cout << "Q: " << printPoly(Q) << std::endl;
  if (P.size() == 1 && Q.size() == 1) {
    T PQ(1, 0);
    PQ[0] = P[0] * Q[0];
    return PQ;
  }

  int mid = P.size() / 2;

  T P_low(P.begin(), P.begin() + mid);
  T P_high(P.begin() + mid, P.end());
  T Q_low(Q.begin(), Q.begin() + mid);
  T Q_high(Q.begin() + mid, Q.end());

  // std::cout << "P_low: " << printPoly(P_low) << std::endl;
  // std::cout << "P_high: " << printPoly(P_high) << std::endl;
  // std::cout << "Q_low: " << printPoly(Q_low) << std::endl;
  // std::cout << "Q_high: " << printPoly(Q_high) << std::endl;

  T sol_l = mult(P_low, Q_low);
  T sol_h = mult(P_high, Q_high);
  T sum_P = sum(P_low, P_high);
  T sum_Q = sum(Q_low, Q_high);
  T sol_m = mult(sum_P, sum_Q);

  // std::cout << "F1: " << printPoly(F) << std::endl;
  sol_m = sub(sol_m, sol_l);
  // std::cout << "F2: " << printPoly(F) << std::endl;
  sol_m = sub(sol_m, sol_h);

  // std::cout << "E: " << printPoly(E) << std::endl;
  // std::cout << "G: " << printPoly(G) << std::endl;
  // std::cout << "F: " << printPoly(F) << std::endl;

  // TODO: proper solution construction
  auto sol = constructSolution(sol_l, sol_m, sol_h);
  // std::cout << "Sol: " << printPoly(sol) << std::endl;
  // F.insert(F.end(), G.begin(), G.end());
  // E.insert(E.end(), F.begin(), F.end());
  return sol;
}
}

#endif

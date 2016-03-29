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
  // std::cout << "P: " << printPoly(P) << std::endl;
  // std::cout << "Q: " << printPoly(Q) << std::endl;
  if (P.size() == 1 && Q.size() == 1) {
    T PQ(1, 0);
    PQ[0] = P[0] * P[0];
    return PQ;
  }

  int mid = P.size() / 2;

  T P_low(P.begin(), P.begin() + mid);
  T P_high(P.begin() + mid, P.end());
  T Q_low(Q.begin(), Q.begin() + mid);
  T Q_high(Q.begin() + mid, Q.end());

  auto E = mult(P_low, Q_low);
  auto G = mult(P_high, Q_high);
  auto sP = sum(P_low, P_high);
  auto sQ = sum(Q_low, Q_high);
  auto F = mult(sP, sQ);

  // std::cout << "F1: " << printPoly(F) << std::endl;
  F = sub(F, E);
  // std::cout << "F2: " << printPoly(F) << std::endl;
  F = sub(F, G);

  std::cout << "E: " << printPoly(E) << std::endl;
  std::cout << "G: " << printPoly(G) << std::endl;
  std::cout << "F: " << printPoly(F) << std::endl;

  // TODO: proper solution construction
  F.insert(F.end(), G.begin(), G.end());
  E.insert(E.end(), F.begin(), F.end());
  return E;
}
}

#endif

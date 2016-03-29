#include "goodrand.hpp"
#include "poly.hpp"
#include <cstdlib>
#include <iostream>

int main() {
  std::vector<double> P;
  std::vector<double> Q;
  for (int i = 0; i < 2; i++) {
    P.push_back(RAND::getRand(1, 5));
    Q.push_back(RAND::getRand(1, 5));
  }

  std::cout << "P: " << POLY::printPoly(P) << std::endl;
  std::cout << "Q: " << POLY::printPoly(Q) << std::endl;
  auto PQ = POLY::naiveMult(P, Q);
  std::cout << "PQ: " << POLY::printPoly(*PQ) << std::endl;

  auto R = POLY::mult(P, Q);
  std::cout << "R: " << POLY::printPoly(R) << std::endl;

  return EXIT_SUCCESS;
}

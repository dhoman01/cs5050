#include "goodrand.hpp"
#include "poly.hpp"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

std::string printRunningTime(std::chrono::duration<double> child_runningtime) {
  std::stringstream ss;
  auto sec =
      std::chrono::duration_cast<std::chrono::seconds>(child_runningtime);
  child_runningtime -= sec;
  auto mill =
      std::chrono::duration_cast<std::chrono::milliseconds>(child_runningtime);
  child_runningtime -= mill;
  auto micro =
      std::chrono::duration_cast<std::chrono::microseconds>(child_runningtime);
  ss << "\n\n*** " << sec.count() << " seconds " << mill.count()
     << " milliseconds and " << micro.count() << " microseconds     ***"
     << std::endl;

  return ss.str();
}

int main() {
  std::cout << "n,Naive (micro sec),Fast (micro sec)" << std::endl;
  for (int n = 32; n < std::pow(2, 100000); n *= 2) {
    std::vector<double> P;
    std::vector<double> Q;
    for (int i = 0; i < n; i++) {
      P.push_back(RAND::getRand(-1.0, 1.0));
      Q.push_back(RAND::getRand(-1.0, 1.0));
    }

    // P.push_back(3);
    // P.push_back(1);
    // Q.push_back(5);
    // Q.push_back(3);
    // std::cout << "n=" << n << std::endl;
    // std::cout << "P: " << POLY::printPoly(P) << std::endl;
    // std::cout << "Q: " << POLY::printPoly(Q) << std::endl;

    auto naiveStart = std::chrono::steady_clock::now();
    auto PQ = POLY::naiveMult(P, Q);
    auto naiveEnd = std::chrono::steady_clock::now();
    // std::cout << "PQ: " << POLY::printPoly(*PQ) << std::endl;

    auto start = std::chrono::steady_clock::now();
    auto R = POLY::mult(P, Q);
    auto end = std::chrono::steady_clock::now();
    // std::cout << "R:  " << POLY::printPoly(R) << std::endl;
    // std::cout << "Naive: " << printRunningTime(naiveEnd - naiveStart)
    //           << std::endl;
    // std::cout << "Fast: " << printRunningTime(end - start) << "\n\n"
    //           << std::endl;
    auto naive = std::chrono::duration_cast<std::chrono::microseconds>(
        naiveEnd - naiveStart);
    auto fast =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << n << "," << naive.count() << "," << fast.count() << std::endl;
  }
  return EXIT_SUCCESS;
}

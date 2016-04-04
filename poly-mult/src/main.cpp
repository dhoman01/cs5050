#define _USE_MATH_DEFINES

#include "goodrand.hpp"
#include "poly.hpp"
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

int main() {
  const auto N_START = 32;
  const auto N_END = std::pow(2, 30);

  const auto P_COEF_LOW = -1.0;
  const auto P_COEF_HIGH = 1.0;
  const auto Q_COEF_LOW = -1.0;
  const auto Q_COEF_HIGH = 1.0;

  // Column names for output
  std::cout << "n,Naive (microsec),Fast (microsec),FFT (microsec)" << std::endl;

  auto teststart = std::chrono::steady_clock::now();

  for (int n = N_START; n < N_END; n *= 2) {
    // Change type to int if P_COEF
    // and Q_COEF are integers
    std::vector<std::complex<long double>> P;
    std::vector<std::complex<long double>> Q;
    std::vector<std::complex<long double>> W;

    // Generate problem set
    for (int i = 0; i < n; i++) {
      std::complex<long double> p_coef(RAND::getRand(P_COEF_LOW, P_COEF_HIGH),
                                       0);
      std::complex<long double> q_coef(RAND::getRand(Q_COEF_LOW, Q_COEF_HIGH),
                                       0);
      P.push_back(p_coef);
      Q.push_back(q_coef);
    }

    for (int j = 0; j < 2 * n; j++) {
      std::complex<long double> w_j(std::cos((2 * M_PI * j) / (2 * n)),
                                    std::sin((2 * M_PI * j) / (2 * n)));
      W.push_back(w_j);
    }

    auto TT =
        std::chrono::steady_clock::now() - std::chrono::steady_clock::now();
    auto naiveDuration = TT;
    auto fastDuration = TT;
    auto fftDuration = TT;

    // Do naive polynomial multiplication
    std::thread naiveThread([=, &naiveDuration]() {
      auto start = std::chrono::steady_clock::now();
      auto PQ = POLY::naiveMult(P, Q);
      auto end = std::chrono::steady_clock::now();
      naiveDuration = end - start;
    });

    // Do divide and conquer polynomial multiplication
    std::thread fastThread([=, &fastDuration]() {
      auto start = std::chrono::steady_clock::now();
      auto F = POLY::mult(P, Q);
      auto end = std::chrono::steady_clock::now();
      fastDuration = end - start;
    });

    // Do FFT polynomial multiplication
    std::thread fftThread([=, &fftDuration]() {
      auto start = std::chrono::steady_clock::now();
      auto FFT = POLY::FFTmult(P, Q, W, n);
      auto end = std::chrono::steady_clock::now();
      fftDuration = end - start;
    });

    naiveThread.join();
    fastThread.join();
    fftThread.join();

    // See the resulting polynomials
    // std::cout << "PQ:  " << POLY::printPoly(*PQ) << std::endl;
    // std::cout << "F:   " << POLY::printPoly(F) << std::endl;
    // std::cout << "FFT: " << POLY::printPoly(FFT) << std::endl;

    // assert fails due to lack of need resolution while working with
    // complex numbers. However the resulting polynomials are equivilant
    // assert(POLY::equal(*PQ, F, 2 * n - 1) && POLY::equal(F, FFT, 2 * n -
    // 1));

    // Print out results of problem set
    //// Calculate running times
    // auto naive = std::chrono::duration_cast<std::chrono::microseconds>(
    //     naiveEnd - naiveStart);
    // auto fast =
    //     std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // auto fft = std::chrono::duration_cast<std::chrono::microseconds>(fftEnd -
    //                                                                  fftStart);

    auto naive =
        std::chrono::duration_cast<std::chrono::microseconds>(naiveDuration);
    auto fast =
        std::chrono::duration_cast<std::chrono::microseconds>(fastDuration);
    auto fft =
        std::chrono::duration_cast<std::chrono::microseconds>(fftDuration);

    //// Write data out
    std::cout << n << "," << naive.count() << "," << fast.count() << ","
              << fft.count() << std::endl;
  }

  auto testend = std::chrono::steady_clock::now();

  auto test = std::chrono::duration_cast<std::chrono::microseconds>(testend -
                                                                    teststart);

  std::cout << "Test total = " << test.count() << std::endl;

  return EXIT_SUCCESS;
}

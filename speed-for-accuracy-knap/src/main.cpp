#include "goodrand.hpp"
#include "knap.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

void set_args(int argc, char *argv[], int &trial_count, int &N, double &K,
              std::string &filename);
std::vector<double> init_size_array(int N);
std::vector<long long int> proj_size_to_int(std::vector<double> &S, int m);
std::pair<double, double> get_cf(const int &N, const double &K,
                                 const std::vector<double> &S, const int &m);
void print_results_to_csv(
    std::vector<std::pair<std::vector<double>, std::vector<double>>> trials,
    std::string filename);
void print_progress(float progress);

int main(int argc, char *argv[]) {
  auto trial_count = 30;
  auto N = 22;
  auto K = 11.0;
  std::string filename = "data.csv";

  if (argc > 1)
    set_args(argc, argv, trial_count, N, K, filename);

  std::vector<std::pair<std::vector<double>, std::vector<double>>> trials;

  std::cout << "Beginning trials: " << std::endl;
  float progress = 0.0;
  for (int i = 0; i < trial_count; i++) {
    std::vector<double> S = init_size_array(N);

    std::pair<double, double> correct_solution = get_cf(N, K, S, 16);
    double T = correct_solution.first;
    double C = correct_solution.second;

    std::vector<double> t;
    t.push_back(T);

    std::vector<double> c;
    c.push_back(C);

    for (int m = 3; m < 12; m++) {
      std::pair<double, double> solution = get_cf(N, K, S, m);
      t.push_back(solution.first);
      c.push_back(solution.second);
      print_progress(progress);
      progress += 1 / (9.0 * trial_count);
    }

    trials.push_back(std::make_pair(t, c));
  }

  print_progress(1);

  std::cout << std::endl;

  print_results_to_csv(trials, filename);

  return EXIT_SUCCESS;
}

void set_args(int argc, char *argv[], int &trial_count, int &N, double &K,
              std::string &filename) {
  if (argc > 1) {
    trial_count = atoi(argv[1]);
  }
  if (argc > 2) {
    N = atoi(argv[2]);
  }
  if (argc > 3) {
    K = atof(argv[3]);
  }
  if (argc > 4) {
    filename = argv[4];
  }
}

std::vector<double> init_size_array(int N) {
  std::vector<double> S;
  for (int i = 0; i < N; i++) {
    S.push_back(RAND::getRand(0.0, 1.0));
  }

  return S;
}

std::vector<long long int> proj_size_to_int(const std::vector<double> &S,
                                            int m) {
  std::vector<long long int> s;
  std::for_each(S.begin(), S.end(), [&s, m](const auto &S_i) {
    long long int s_i = std::floor(S_i * std::pow(10, m));
    assert(s_i >= 0);
    s.push_back(s_i);
  });

  return s;
}

std::pair<double, double> get_cf(const int &N, const double &K,
                                 const std::vector<double> &S, const int &m) {
  double offset = std::pow(10, m);
  long long int k = std::floor(K * std::pow(10, m));
  auto s = proj_size_to_int(S, m);
  std::vector<std::unordered_map<long long int, long long int>> cache(
      N + 1, std::unordered_map<long long int, long long int>());

  assert(k > 0);
  auto start = std::chrono::steady_clock::now();
  long long int c = Knapsack::CF_knap(N, k, s, cache);
  auto end = std::chrono::steady_clock::now();
  auto t = end - start;

  return std::make_pair(
      std::chrono::duration_cast<std::chrono::microseconds>(t).count(),
      (double)(c / std::pow(10, m)));
}

void print_results_to_csv(
    std::vector<std::pair<std::vector<double>, std::vector<double>>> trials,
    std::string filename) {
  std::ofstream file(filename);

  file << "trial,time,error" << std::endl;
  auto i = 1;
  std::for_each(trials.begin(), trials.end(), [&](auto &p) {
    auto t = p.first;
    auto c = p.second;
    auto C = c[0];
    for (int j = 0; j < c.size(); j++) {
      file << i << "," << t[j] << "," << std::pow(C - c[j], 2) << std::endl;
    }

    i++;
  });

  file.close();

  std::cout << "Results are located at: " << get_current_dir() << "/"
            << filename << std::endl;
}

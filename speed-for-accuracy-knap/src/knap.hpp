#ifndef KNAP_HPP
#define KNAP_HPP

#include <limits>
#include <unordered_map>
#include <vector>

namespace Knapsack {

// Recursive Closest Fit Knapsack
auto CF_knap(int n, int k, const std::vector<int> &s) {
  if (k < 0)
    return std::numeric_limits<int>::max();
  if (n == 0)
    return k;
  return std::min(CF_knap(n - 1, k, s), CF_knap(n - 1, k - s[n], s));
}

// Memoizing Closest Fit Knapsack
template <typename T>
auto CF_knap(int n, T k, const std::vector<T> &s,
             std::vector<std::unordered_map<T, T>> &cache) {
  if (cache[n].find(k) != cache[n].end())
    return cache[n].at(k);
  if (k < 0)
    return std::numeric_limits<T>::max();
  if (n == 0)
    return k;
  long long int value =
      std::min(CF_knap(n - 1, k, s, cache), CF_knap(n - 1, k - s[n], s, cache));
  cache[n].insert(std::make_pair(k, value));
  return value;
}
}

#endif

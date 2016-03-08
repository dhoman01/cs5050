#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <sstream>

int match(char a, char b)
{
  if(a == b) return 1;
  else if(a == 'A' && b == 'G') return -3;
  else if(a == 'C' && b == 'T') return -3;
  else return -1;
}

// opt_traceback
// create vector<int> size of A
// store "y" coordinate produced by dp_opt_space
// Use this map to rebuild alignment

/*
P(x) = int[]
Q(x) = int[]
PQ(x) = int[]
*/
for(int i = 0; i < N; i++)
  for(int j = 0; j < N; j++)
    PQ[i+j] += P[i] * P[j];

stringstream& dp_traceback(std::vector<std::vector<int>> cache, stringstream& ss, int i, int j, std::string A, std::string B)
{
  int subD = cache[i-1][j] - 2;
  int subI = cache[i][j-1] - 2;
  int Swap = cache[i-1][j-1];

}

std::vector<std::vector<int>> dp_align(std::string a, std::string b)
{
  std::vector<std::vector<int>> cache = std::vector<std::vector<int>>(a.size() + 1, std::vector<int>(b.size() + 1, 0));

  for(int i = 0; i <= a.size(); i++)
  {
    for(int j = 0; j <= b.size(); j++)
    {
      if(i == 0) cache[0][j] = j * -2;
      else if(j == 0) cache[i][0] = i * -2;
      else
      {
        int subD = cache[i-1][j] - 2;
        int subI = cache[i][j-1] - 2;
        int Swap = cache[i-1][j-1] + match(a[i-1],b[j-1]);

        int max = std::max(subD, std::max(subI, Swap));
        cache[i][j] = max;
      }
    }
  }

  return cache;
}

std::vector<std::vector<int>> dp_align_opt(std::string a, std::string b)
{
  std::vector<std::vector<int>> cache = std::vector<std::vector<int>>(2, std::vector<int>(b.size() + 1, 0));

  for(int i = 0; i <= a.size(); i++)
  {
    for(int j = 0; j <= b.size(); j++)
    {
      if(i == 0) cache[1][j] = j * -2;
      else if(j == 0) cache[1][0] = i * -2;
      else
      {
        int subD = cache[0][j] - 2;
        int subI = cache[1][j-1] - 2;
        int Swap = cache[0][j-1] + match(a[i-1],b[j-1]);

        int max = std::max(subD, std::max(subI, Swap));
        cache[1][j] = max;
      }
    }
    if(i < a.size())cache[0] = cache[1];
  }

  return cache;
}

int main()
{


  return EXIT_SUCCESS;
}

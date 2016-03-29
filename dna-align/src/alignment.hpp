#ifndef ALIGNMENT
#define ALIGNMENT

#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

namespace align
{
  // this function returns the score of
  // two characters. -999999 if there is
  // an error
  int match(char a, char b)
  {
    if(a == b) return 5;
    else if(a == 'A' && b == 'G') return -2;
    else if(a == 'A' && (b == 'C' || b == 'T')) return -1;
    else if(a == 'A' && b == '-') return -3;
    else if(a == 'C' && b == 'A') return -1;
    else if(a == 'C' && b == 'G') return -3;
    else if(a == 'C' && b == 'T') return -2;
    else if(a == 'C' && b == '-') return -4;
    else if(a == 'G' && (b == 'A' || b == 'T' || b == '-')) return -2;
    else if(a == 'G' && b == 'C') return -3;
    else if(a == 'T' && (b == 'A' || b == '-')) return -1;
    else if(a == 'T' && (b == 'C' || b == 'G')) return -2;
    else {
      switch(b){
        case 'A':
          return -3;
          break;
        case 'C':
          return -4;
          break;
        case 'G':
          return -2;
          break;
        case 'T':
          return -1;
          break;
      }
    }
    return -99999;
  }

  // Returns the parition index for the Hirshberg algorithm
  int partition(std::vector<int>& scoreL, std::vector<int>& scoreR)
  {
    int i = 0;
    int max = -999999;
    int index = -1;
    std::for_each(scoreR.rbegin(),scoreR.rend(),[&](auto score){
      int sum = score + scoreL[i];
      if(sum > max)
      {
        max = sum;
        index = i;
      }
      i++;
    });

    return index;
  }

  // Naive traceback using a full solution space
  std::string dp_traceback(std::vector<std::vector<int>> cache, int i, int j, std::string& A, std::string& B)
  {
    std::stringstream ss;
    if(i < 1)
    {
      while(j > 0)
      {
        ss << "- -> " << B[j-1] << " (" << match('-',B[j-1]) << ")" << std::endl;
        j--;
      }
      return ss.str();
    }
    if(j < 1)
    {
      while (i > 0)
      {
        ss << A[i-1] << " -> - (" << match(A[i-1],'-') << ")" <<  std::endl;
        i--;
      }
      return ss.str();
    }
    int subD = cache[i-1][j] + match(A[i-1],'-');
    int subI = cache[i][j-1] + match('-',B[j-1]);
    int Swap = cache[i-1][j-1] + match(A[i-1], B[j-1]);
    int max = std::max(subD,std::max(subI,Swap));
    if(max == subD)
    {
      ss << dp_traceback(cache, i-1, j, A, B) << A[i-1] << " -> - (" << match(A[i-1],'-') << ")" <<  std::endl;
      return ss.str();
    }
    else if(max == subI)
    {
      ss << dp_traceback(cache, i, j-1, A, B) << "- -> " << B[j-1] << " (" << match('-',B[j-1]) << ")" << std::endl;
      return ss.str();
    }
    else if(max == Swap)
    {
      ss << dp_traceback(cache, i-1, j-1, A, B) << A[i-1] << " -> " << B[j-1] << " (" << match(A[i-1], B[j-1]) << ")" << std::endl;
      return ss.str();
    }
    return ss.str();
  }

  // Naive algorithm to find alignment score -- uses O(n^2) space
  std::string dp_align(std::string& a, std::string& b)
  {
    std::vector<std::vector<int>> cache = std::vector<std::vector<int>>(a.size() + 1, std::vector<int>(b.size() + 1, 0));

    for(int i = 0; i <= a.size(); i++)
    {
      for(int j = 0; j <= b.size(); j++)
      {
        if(i == 0)
        {
          int sum = 0;
          for(int k = 0; k < j; k++)
            sum += match('-', b[k]);
          cache[0][j] = sum;
        }
        else if(j == 0)
        {
          int sum = 0;
          for(int k = 0; k < i; k++)
            sum += match(a[k], '-');
          cache[i][0] = sum;
        }
        else
        {
          int subD = cache[i-1][j] + match(a[i-1], '-');
          int subI = cache[i][j-1] + match('-', b[j-1]);
          int Swap = cache[i-1][j-1] + match(a[i-1],b[j-1]);
          int max = std::max(subD, std::max(subI, Swap));
          cache[i][j] = max;
        }
      }
    }

    return dp_traceback(cache,a.size(),b.size(),a,b);
  }

  // optimized dp_align algorithm to use O(b.size()) space
  std::vector<int> dp_align_opt(std::string& a, std::string& b)
  {
    std::vector<std::vector<int>> cache = std::vector<std::vector<int>>(2, std::vector<int>(b.size() + 1, 0));

    for(int i = 0; i <= a.size(); i++)
    {
      for(int j = 0; j <= b.size(); j++)
      {
        if(i == 0)
        {
          int sum = 0;
          for(int k = 0; k < j; k++)
            sum += match('-', b[k]);
          cache[1][j] = sum;
        }
        else if(j == 0)
        {
          int sum = 0;
          for(int k = 0; k < i; k++)
            sum += match(a[k], '-');
          cache[1][0] = sum;
        }
        else
        {
          int subD = cache[0][j] + match(a[i-1], '-');
          int subI = cache[1][j-1] + match('-', b[j-1]);
          int Swap = cache[0][j-1] + match(a[i-1],b[j-1]);

          int max = std::max(subD, std::max(subI, Swap));
          cache[1][j] = max;
        }
      }
      if(i < a.size())cache[0] = cache[1];
    }

    return cache[1];
  }

  // Hirshberg algorithm returning the alignment as a std::string
  std::string Hirshberg(std::string& X, std::string& Y)
  {
    std::stringstream ss;
    if(X.size() == 0)
    {
      for(int i = 0; i < Y.size(); i++)
      {
        ss << "- -> " << Y[i] << " (" << match('-',Y[i]) << ")" <<  std::endl;
      }
      return ss.str();
    }
    else if(Y.size() == 0)
    {
      for(int i = 0; i < X.size(); i++)
      {
        ss << X[i] << " -> - (" << match(X[i],'-') << ")" <<  std::endl;
      }
      return ss.str();
    }
    else if(X.size() == 1 || Y.size() == 1)
    {
      return dp_align(X,Y);
    }
    else
    {
      int xlen = X.size();
      int ylen = Y.size();

      int xmid = std::ceil(X.size() / 2);

      std::string lX = X.substr(0,xmid);
      std::string rX = X.substr(xmid);
      std::string rY = Y;
      std::reverse(rX.begin(), rX.end());
      std::reverse(rY.begin(), rY.end());

      std::vector<int> scoreL = dp_align_opt(lX,Y);
      std::vector<int> scoreR = dp_align_opt(rX, rY);

      int ymid = partition(scoreL, scoreR);

      std::string lY = Y.substr(0,ymid);
      rX = X.substr(xmid);
      rY = Y.substr(ymid);
      ss << Hirshberg(lX,lY) << Hirshberg(rX, rY);
    }
    return ss.str();
  }

  void test_upper_bound(int size)
  {
    int* test = new int[size * size];
    delete test;
  }

  std::string generate_random_sequence(int u)
  {
    char n[] = {'A','G','C','T'};
    std::string sequence;
    for(int i = 0; i < u; i++)
    {
      sequence += n[rand() % 3];
    }

    return sequence;
  }
}

#endif

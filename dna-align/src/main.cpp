#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <chrono>

int match(char a, char b);

std::string Hirshberg(std::string& X, std::string& Y);
int partition(std::vector<int>& scoreL, std::vector<int>& scoreR);

std::string dp_traceback(std::vector<std::vector<int>> cache, int i, int j, std::string& A, std::string& B);

// NW algorithm. Calls cp_traceback
std::string dp_align(std::string& a, std::string& b);

// Equivalent to NWScore
std::vector<int> dp_align_opt(std::string& a, std::string& b);

std::string read_mDNA(std::string filename);
void write_alignment(std::string filename, std::string& alignment);

void compareTo(std::string source_file, int numberOfFiles, std::string& source_mdna);
/*
P(x) = int[]
Q(x) = int[]
PQ(x) = int[]
*/
// for(int i = 0; i < N; i++)
//   for(int j = 0; j < N; j++)
//     PQ[i+j] += P[i] * P[j];
void doPrintRunningTime(std::chrono::duration<double> child_runningtime)
{
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(child_runningtime);
  child_runningtime -= sec;
  auto mill = std::chrono::duration_cast<std::chrono::milliseconds>(child_runningtime);
  child_runningtime -= mill;
  auto micro = std::chrono::duration_cast<std::chrono::microseconds>(child_runningtime);
  std::cout << "***     Time spent executing: " << sec.count() << " seconds ";
  std::cout << mill.count() << " milliseconds and " << micro.count() << " microseconds     ***" << std::endl;
}

int main()
{
  std::string proto_human = read_mDNA("../src/proto-human.mdna");
  auto baseline_score = dp_align_opt(proto_human, proto_human);

  std::cout << "Baseline " << baseline_score[proto_human.size()] << std::endl;

  auto start = std::chrono::steady_clock::now();
  compareTo("../src/neandertal/neandertal",1,proto_human);
  compareTo("../src/human/human",10,proto_human);
  auto end = std::chrono::steady_clock::now();

  doPrintRunningTime(end - start);

  return EXIT_SUCCESS;
}

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
}

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
    int xmid = std::ceil(X.size() / 2);
    int ylen = Y.size();
    auto lX = X.substr(0,xmid);
    auto scoreL = dp_align_opt(lX,Y);
    auto rX = X.substr(xmid);
    auto rY = Y;
    std::reverse(rX.begin(), rX.end());
    std::reverse(rY.begin(), rY.end());
    auto scoreR = dp_align_opt(rX, rY);

    int ymid = partition(scoreL, scoreR);
    auto lY = Y.substr(0,ymid);
    rX = X.substr(xmid);
    rY = Y.substr(ymid);
    ss << Hirshberg(lX,lY) << Hirshberg(rX, rY);
  }
  return ss.str();
}

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
}

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

std::string read_mDNA(std::string filename)
{
  std::stringstream ss;

  std::ifstream file;
  file.open(filename);
  if(file.is_open())
  {
    while(!file.eof())
    {
      char c = file.get();
      switch (c)
      {
        case '\n':
        case '\r':
        case 'R':
        case 'Y':
        case 'N':
          break;
        default:
          ss << c;
          break;
      }
    }
  }
  else
  {
    std::cerr << "Couldn't find file " << filename << std::endl;
  }
  std::string result = ss.str();
  return result.substr(0,result.size()-1);
}

void write_alignment(std::string filename, std::string& alignment)
{
  std::ofstream file;
  file.open(filename);
  if(file.is_open()) file << alignment;
  file.close();
}

void compareTo(std::string source_file, int numberOfFiles, std::string& source_mdna)
{
  std::vector<int> scores;
  for(int i = 1; i <= numberOfFiles; i++)
  {
    std::stringstream ss;
    ss << source_file << "-" << i << ".mdna";
    std::string sequence = read_mDNA(ss.str());
    scores.push_back(dp_align_opt(source_mdna,sequence)[sequence.size()]);
    auto alignment = Hirshberg(source_mdna, sequence);
    write_alignment(ss.str().append(".alignment"), alignment);
  }
  for(int i = 1; i <= numberOfFiles; i++)
  {
    std::cout << "Proto-Human to " << source_file << "-" << i << " score: " << scores[i-1] << std::endl;
  }
}
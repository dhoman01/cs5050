#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

std::string A = "";
std::string B = "";


int MED_R(int i, int j)
{
  if(i == 0) return j;
  if(j == 0) return i;
  int a = MED_R(i - 1, j) + 1;
  int b = MED_R(i, j - 1) + 1;
  int extra = (A[i] == B[j]) ? 0 : 1;
  int c = MED_R(i - 1, j - 1) + extra;
  return std::min(a,std::min(b,c));
}

int MED_DP(int a, int b)
{
  std::vector<std::vector<int>> cache (a + 1, std::vector<int> (b + 1, 0));

  for(int i = 0; i <= a; i++)
  {
    cache[i][0] = a;
  }

  for(int j = 0; j <= b; j++)
  {
    cache[0][j] = b;
  }

  cache[0][0] = 0;

  for(int i = 1; i <= a; i++)
  {
    for(int j = 1; j <= b; j++)
    {
      int one = cache[i - 1][j] + 1;
      int two = cache[i][j - 1] + 1;
      int three = cache[i - 1][j - 1] + ((A[i] == B[j]) ? 0 : 1);
      cache[i][j] = std::min(one, std::min(two, three));
    }
  }

  return cache[a][b];
}

void do_recursive_test(std::vector<std::string> misspelt_words, std::vector<std::string> correct_spelling)
{
  std::vector<int> counts (misspelt_words.size(),0);

  int counter = 0;
  std::for_each(misspelt_words.begin(), misspelt_words.end(),[=,&counts,&counter](auto& word){
    A = word;
    B = correct_spelling[counter];
    std::cout << "working on words " << A << " and " << B;
    int med = MED_R(A.size(), B.size());
    std::cout << " " << med << std::endl;
    counts[med] += 1;
    counter++;
    std::cout << "counter " << counter << std::endl;
  });

  std::ofstream out_file("recursive_results.out");
  out_file << "n     count" << std::endl;
  out_file << "===================" << std::endl;
  for(int i = 0; i < counts.size(); i++)
  {
    out_file << i << "     " << counts[i] << std::endl;
  }
}

void do_dp_test(std::vector<std::string> misspelt_words, std::vector<std::string> correct_spelling)
{
  std::vector<int> counts (misspelt_words.size(),0);

  int counter = 0;
  std::for_each(misspelt_words.begin(), misspelt_words.end(),[=,&counts,&counter](auto& word){
    A = word;
    B = correct_spelling[counter];
    int med = MED_DP(A.size(), B.size());
    counts[med] += 1;
    counter++;
    std::cout << "counter and med " << counter << " " << med << std::endl;
  });

  std::ofstream out_file("dp_results.out");
  out_file << "n     count" << std::endl;
  out_file << "===================" << std::endl;
  for(int i = 0; i < counts.size(); i++)
  {
    out_file << i << "     " << counts[i] << std::endl;
  }
}


int main()
{
  std::cout << "Reading in words..." << std::endl;
  std::vector<std::string> misspelt_words;
  std::vector<std::string> correct_spelling;
  std::ifstream in_file("words.in");
  std::string line;
  if(in_file.is_open())
  {
    while(std::getline(in_file, line))
    {
      std::size_t first_del = 0;
      std::size_t second_del = 0;
      first_del = line.find("->");
      std::string a = line.substr(0, first_del);
      std::string b = line.substr(first_del + 2);
      misspelt_words.push_back(a);
      if((second_del = b.find(", ")) != std::string::npos)
      {
        misspelt_words.push_back(a);
        correct_spelling.push_back(b.substr(0, second_del));
        correct_spelling.push_back(b.substr(second_del + 2));
      }
      else
      {
        correct_spelling.push_back(b);
      }
    }
  }

  std::cout << "Finished reading in words" << std::endl;

  char do_recu = 'n';
  std::cout << "Do both tests? y or n" << std::endl;
  std::cin >> do_recu;

  if(do_recu == 'y')
  {
    do_recursive_test(misspelt_words, correct_spelling);
    do_dp_test(misspelt_words, correct_spelling);
  }
  else
  {
    do_dp_test(misspelt_words, correct_spelling);
  }

  return EXIT_SUCCESS;
}

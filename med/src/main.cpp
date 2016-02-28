#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

std::string A = ""; // Global to store current misspelt word
std::string B = ""; // Global to store current correct word


int MED_R(int i, int j)
{
  //Simple answers
  if(i == 0) return j;
  if(j == 0) return i;

  int a = MED_R(i - 1, j) + 1; // possible deletion
  int b = MED_R(i, j - 1) + 1; // possible inserstion
  int c = MED_R(i - 1, j - 1) + ((A[i] == B[j]) ? 0 : 1); // possible subsition or do nothing

  // Return the min of the three possible word edits
  return std::min(a,std::min(b,c));
}

int MED_DP(int a, int b)
{
  // cache covering solution space
  std::vector<std::vector<int>> cache (a + 1, std::vector<int> (b + 1, 0));

  // Store simple solutions
  for(int i = 0; i <= a; i++)
  {
    cache[i][0] = a;
  }

  for(int j = 0; j <= b; j++)
  {
    cache[0][j] = b;
  }

  cache[0][0] = 0;

  // Scan problem space by column
  for(int i = 1; i <= a; i++)
  {
    for(int j = 1; j <= b; j++)
    {
      int one = cache[i - 1][j] + 1; // possible deletion
      int two = cache[i][j - 1] + 1; // possible inserstion
      int three = cache[i - 1][j - 1] + ((A[i - 1] == B[j - 1]) ? 0 : 1); // possible subsition or do nothing

      // Store the min of the three possible word edits
      cache[i][j] = std::min(one, std::min(two, three));
    }
  }

  // return solution
  return cache[a][b];
}

void do_recursive_test(std::vector<std::string> misspelt_words, std::vector<std::string> correct_spelling)
{
  // vector to store the counts of MEDs and associated words
  std::vector<std::pair<int,std::vector<std::pair<std::string,std::string>>>> counts (14,std::make_pair(0,std::vector<std::pair<std::string,std::string>>()));

  int counter = 0;
  int max = 0;  // Stores maximal MED

  // Iterate over the misspelt words and call MED_DP to
  // calculate minimum edit distance
  std::for_each(misspelt_words.begin(), misspelt_words.end(),[=,&counts,&counter,&max](auto& word){
    A = word;
    B = correct_spelling[counter];

    // do to MED_R being O(2^n)
    // we throw out words that will
    // take too long. Feel free to adjust
    // if you want to wait for it.
    if(A.size() < 14){
      int med = MED_R(A.size(), B.size());
      if(counts[med].first > max) max = med;
      counts[med].first += 1;
      counts[med].second.push_back(std::make_pair(A,B));
      counter++;
      if(counter % 750 == 0)
        std::cout << "counter " << counter << "..."<< std::endl;
    }
  });

  // Print out table of MED sizes and counts
  // of word pairs that correlate
  std::ofstream out_file("recursive_results.out");
  out_file << "n     count" << std::endl;
  out_file << "===================" << std::endl;
  for(int i = 0; i < counts.size(); i++)
  {
    out_file << i << "     " << counts[i].first << std::endl;
  }

  // Print out the Maximal MED and word pairs
  out_file << "\n\nMaximal MED is " << max << std::endl;
  out_file << "==========================" << std::endl;
  std::for_each(counts[max].second.begin(), counts[max].second.end(),[&out_file](auto& pair){
    out_file << pair.first << "->" << pair.second << std::endl;
  });

  out_file.close();
}

void do_dp_test(std::vector<std::string> misspelt_words, std::vector<std::string> correct_spelling, int max_word_size)
{
  // vector to store the counts of MEDs and associated words
  std::vector<std::pair<int,std::vector<std::pair<std::string,std::string>>>> counts (max_word_size,std::make_pair(0,std::vector<std::pair<std::string,std::string>>()));

  int counter = 0;
  int max = 0;  // Stores maximal MED

  // Iterate over the misspelt words and call MED_DP to
  // calculate minimum edit distance
  std::for_each(misspelt_words.begin(), misspelt_words.end(),[=,&counts,&counter,&max](auto& word){
    A = word;
    B = correct_spelling[counter];
    int med = MED_DP(A.size(), B.size());

    if(med > max) max = med;
    counts[med].first += 1;
    counts[med].second.push_back(std::make_pair(A,B));
    counter++;
  });

  // Print out table of MED sizes and counts
  // of word pairs that correlate
  std::ofstream out_file("dp_results.out");
  out_file << "n     count" << std::endl;
  out_file << "===================" << std::endl;
  int sum = 0;
  for(int i = 0; i < counts.size(); i++)
  {
    out_file << i << "     " << counts[i].first << std::endl;
    sum += counts[i].first;
  }

  // Print out the Maximal MED and word pairs
  out_file << "\n\nMaximal MED is " << max << std::endl;
  out_file << "==========================" << std::endl;
  std::for_each(counts[max].second.begin(), counts[max].second.end(),[&out_file](auto& pair){
    out_file << pair.first << "->" << pair.second << std::endl;
  });

  out_file.close();
  std::cout << "Sum counts: " << sum << std::endl;
}


int main()
{
  std::cout << "Reading in words..." << std::endl;
  /* *************************************************************************************
   * The following section of code parses the list of words found at:                    *
   * https://en.wikipedia.org/wiki/Wikipedia%3aLists_of_common_misspellings/For_machines *
   **************************************************************************************/
  std::vector<std::string> misspelt_words;
  std::vector<std::string> correct_spelling;

  int max_word_size = 0; // Used in initializing vector in do_dp_test

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
      if(a.size() > max_word_size) max_word_size = a.size();
      if((second_del = b.find(", ")) != std::string::npos)
      {
        std::istringstream ss (b);
        while(ss){
            std::string b_n;
            if(!std::getline(ss, b_n, ',')) break;
            misspelt_words.push_back(a);
            correct_spelling.push_back(b_n);
        }
      }
      else
      {
        misspelt_words.push_back(a);
        correct_spelling.push_back(b);
      }
    }
  }

  std::cout << "Finished reading in words" << std::endl;

  char do_recu = 'n';
  std::cout << "Do both tests (recursion only attempts words with length less than 14)? y or n" << std::endl;
  std::cin >> do_recu;

  std::cout << "Beginning tests" << std::endl;
  if(do_recu == 'y')
  {
    do_recursive_test(misspelt_words, correct_spelling);
    do_dp_test(misspelt_words, correct_spelling, max_word_size);
  }
  else
  {
    do_dp_test(misspelt_words, correct_spelling, max_word_size);
  }
  std::cout << "Size of list: " << misspelt_words.size() << std::endl;

  std::cout <<"Tests are finished..." << std::endl;

  return EXIT_SUCCESS;
}

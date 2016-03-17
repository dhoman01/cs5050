#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <map>
#include <iomanip>
#include "alignment.hpp"
#include <thread>
#include <unistd.h>
#include <time.h>

std::string read_mDNA(std::string filename);
void write_alignment(std::string filename, std::string& alignment);

// void compareTo(std::string source_file, int numberOfFiles, std::string& source_mdna);

void pairWise(std::vector<std::string> source_files);

void doSpaceTest();

void compareHumanToNeandertal();
void compareHumanDiversity();
void compareHumanToGreatApes();

void doPrintRunningTime(std::chrono::duration<double> child_runningtime);
void printPairWiseResults(std::map<std::string, std::map<std::string,int>> scores);

std::string fileToNameMap(std::string filename);

template<typename T>
void printElement(char align, T t, const int& width, char fill)
{
    std::cout << (align == 'L' ? std::left : std::right) << std::setw(width) << std::setfill(fill) << t;
}

void domDNAStudy()
{
  auto start = std::chrono::steady_clock::now();
  std::cout << "Proto-Human to Neandertal" << std::endl;
  printElement('L',"+", 50,'+');
  std::cout << std::endl;
  compareHumanToNeandertal();
  // compareTo("../src/neandertal/neandertal",1,proto_human);
  std::cout << "\nHuman Diversity" << std::endl;
  printElement('L',"+", 50,'+');
  std::cout << std::endl;
  compareHumanDiversity();
  // compareTo("../src/human/human",10,proto_human);
  std::cout << "\nProto-Human to the Great Apes " << std::endl;
  printElement('L',"+", 50,'+');
  std::cout << std::endl;
  compareHumanToGreatApes();
  // compareTo("../src/great-apes/ape",6,proto_human);
  auto end = std::chrono::steady_clock::now();

  doPrintRunningTime(end - start);
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  if(argc > 1)
  {
    if(std::string(argv[1]) == "-f")
    {
      auto file = fopen("log.out", "w");

      dup2(fileno(file),STDOUT_FILENO);
    }
    else
    {
      std::cerr << "-f write STDOUT to log file instead of console" << std::endl;
      return EXIT_FAILURE;
    }
  }

  doSpaceTest();
  domDNAStudy();

  return EXIT_SUCCESS;
}

void doSpaceTest()
{
  for(int i = 1; i < 10; i++)
  {
    try
    {
      float size = std::pow(10.0,i);
      std::cout << "Doing test with sequence size 10^" << i << std::endl;
      align::test_upper_bound(size);
    }
    catch (std::bad_alloc& badAlloc)
    {
      std::cout << "Caught bad_alloc exception at size 10^" << i << " " << badAlloc.what() << std::endl;
    }

  }
}

void pairWise(std::vector<std::string> source_files)
{
  // Map to store the pairwise scores
  std::map<std::string,std::map<std::string,int>> scores;

  // iterate over each source file and compare it to
  // all other files including its self
  std::for_each(source_files.begin(), source_files.end(),[&](auto& filename){
    std::string sequence1 = read_mDNA(filename);
    std::string fn1 = fileToNameMap(filename);
    std::for_each(source_files.begin(), source_files.end(),[&](auto& filename2){
      std::string sequence2 = read_mDNA(filename2);
      std::string fn2 = fileToNameMap(filename2);
      std::vector<int> score = align::dp_align_opt(sequence1,sequence2);
      scores[fn1][fn2] = score[sequence2.size()];
    });
  });

  printPairWiseResults(scores);
}

void printPairWiseResults(std::map<std::string,std::map<std::string,int>> scores)
{
  std::cout << "\n" << std::endl;
  std::cout << "Pairwise comparison" << std::endl;
  printElement('R'," ", 11, ' ');
  std::cout << "| ";
  std::for_each(scores.begin(), scores.end(),[](auto it){
    printElement('R',it.first, 11, ' ');
  });

  std::cout << std::endl;
  printElement('R',"-", (scores.size() + 1) * 11, '-');
  std::cout << std::endl;

  std::for_each(scores.begin(),scores.end(),[&scores](auto it){
    printElement('L',it.first,11, ' ');
    std::cout << "|";
    std::for_each(it.second.begin(),it.second.end(),[](auto it){
      printElement('R',it.second,11, ' ');
    });
    std::cout << std::endl;
  });
}

void compareHumanToNeandertal()
{
  std::string proto_human = read_mDNA("../src/proto-human.mdna");
  std::string neandertal = read_mDNA("../src/neandertal/neandertal-1.mdna");
  std::vector<int> score = align::dp_align_opt(proto_human,neandertal);
  std::string alignment = align::Hirshberg(proto_human, neandertal);

  // Write alignment to file asychronously
  std::thread t([alignment](){
    std::string align = alignment;
    write_alignment("../src/neandertal/neandertal-1.mdna.alignment", align);
  });
  t.detach();

  std::cout << "Proto-Human to Neandertal score: " << score[neandertal.size()] << std::endl;
}

void compareHumanDiversity()
{
  // Build list of filenames
  std::vector<std::string> filenames;
  for(int i = 1; i <= 10; i++)
  {
    std::stringstream ss;
    ss << "../src/human/human-" << i << ".mdna";
    filenames.push_back(ss.str());
  }

  // Do pairwise comparison
  pairWise(filenames);
}

void compareHumanToGreatApes()
{
  // Build list of filenames
  std::vector<std::string> filenames;
  filenames.push_back("../src/proto-human.mdna");
  for(int i = 1; i <= 6; i++)
  {
    std::stringstream ss;
    ss << "../src/great-apes/ape-" << i << ".mdna";
    filenames.push_back(ss.str());
  }

  // Do pairwise comparison
  pairWise(filenames);

  std::string proto_human = read_mDNA("../src/proto-human.mdna");
  for(int i = 1; i <= 6; i++)
  {
    std::string filename = filenames[i];
    std::string sequence = read_mDNA(filename);
    std::string alignment = align::Hirshberg(proto_human,sequence);
    filename.append(".alignment");

    // Write alignment to file asychronously
    // except for the last file, will join on that thread
    std::thread t([filename,alignment](){
      std::string align = alignment;
      write_alignment(filename, align);
    });
    if(i < 6) t.detach();
    else
    {
      std::cout << "\nWaiting on last file to write..." << std::endl;
      t.join();
    }
  }
}

void doPrintRunningTime(std::chrono::duration<double> child_runningtime)
{
  auto sec = std::chrono::duration_cast<std::chrono::seconds>(child_runningtime);
  child_runningtime -= sec;
  auto mill = std::chrono::duration_cast<std::chrono::milliseconds>(child_runningtime);
  child_runningtime -= mill;
  auto micro = std::chrono::duration_cast<std::chrono::microseconds>(child_runningtime);
  std::cout << "\n\n***     Time spent executing: " << sec.count() << " seconds ";
  std::cout << mill.count() << " milliseconds and " << micro.count() << " microseconds     ***" << std::endl;
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

// Function used to compare a list of squences to a source sequence
/*void compareTo(std::string source_file, int numberOfFiles, std::string& source_mdna)
{
  std::map<std::string,int> scores;
  scores["Human"] = source_mdna.size() * 5;
  for(int i = 1; i <= numberOfFiles; i++)
  {
    std::stringstream ss;
    ss << source_file << "-" << i << ".mdna";
    std::string sequence = read_mDNA(ss.str());
    std::string fn = fileToNameMap(ss.str());
    auto score = dp_align_opt(source_mdna,sequence);
    scores[fn] = score[sequence.size()];
    auto alignment = Hirshberg(source_mdna, sequence);
    write_alignment(ss.str().append(".alignment"), alignment);
  }

  std::cout << "Scores Table" << std::endl;
  std::for_each(scores.begin(), scores.end(),[](auto it){
    printElement('L',it.first, 10, ' ');
  });

  std::cout << std::endl;
  printElement('R',"-", scores.size() * 10, '-');
  std::cout << std::endl;

  std::for_each(scores.begin(), scores.end(),[](auto it){
    printElement('L',it.second,10,' ');
  });

  std::cout << "\n" << std::endl;
  std::cout << "Pairwise comparison" << std::endl;
  printElement('R'," ", 10, ' ');
  std::cout << "| ";
  std::for_each(scores.begin(), scores.end(),[](auto it){
    printElement('R',it.first, 10, ' ');
  });

  std::cout << std::endl;
  printElement('R',"-", (scores.size() + 1) * 10, '-');
  std::cout << std::endl;

  std::for_each(scores.begin(),scores.end(),[&scores](auto it){
    printElement('L',it.first,10, ' ');
    std::cout << "|";
    int scr = it.second;
    std::for_each(scores.begin(),scores.end(),[&scr](auto it){
      printElement('R',std::abs(scr - it.second),10, ' ');
    });
    std::cout << std::endl;
  });
}*/

std::string fileToNameMap(std::string filename)
{
  if(filename.find("proto") != std::string::npos) return "Proto-Human";
  if(filename.find("neandertal") != std::string::npos) return "Neandertal";
  if(filename.find("human") != std::string::npos)
  {
    if(filename.find("1") != std::string::npos) return "Italy";
    else if(filename.find("2") != std::string::npos) return "Dutch";
    else if(filename.find("3") != std::string::npos) return "Egypt";
    else if(filename.find("4") != std::string::npos) return "Ethiopia";
    else if(filename.find("5") != std::string::npos) return "Indian";
    else if(filename.find("6") != std::string::npos) return "Israel";
    else if(filename.find("7") != std::string::npos) return "Navajo";
    else if(filename.find("8") != std::string::npos) return "S. Africa";
    else if(filename.find("9") != std::string::npos) return "Spain";
    else if(filename.find("10") != std::string::npos) return "Tonga";
  }
  if(filename.find("ape") != std::string::npos)
  {
    if(filename.find("1") != std::string::npos) return "Baboon";
    else if(filename.find("2") != std::string::npos) return "Bonobo";
    else if(filename.find("3") != std::string::npos) return "Chimp 1";
    else if(filename.find("4") != std::string::npos) return "Chimp 2";
    else if(filename.find("5") != std::string::npos) return "Gorilla 1";
    else if(filename.find("6") != std::string::npos) return "Gorilla 2";
  }

  return "Unknown";
}

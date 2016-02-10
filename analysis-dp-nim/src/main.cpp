#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>
#include "timer.h"

using namespace std;

// Naive recursive algorithm
bool Win_R(int a, int b, int c){
  if( a == 0 && b == 0 && c == 0) return false;

  for(int i = 1; i < a + 1; i++)
    if(!Win_R(a - i, b, c)) return true;
  for(int i = 1; i < b + 1; i++)
    if(!Win_R(a, b - i, c)) return true;
  for(int i = 1; i < c + 1; i++)
    if(!Win_R(a, b, c - i)) return true;

  return false;
}

// Recursion with memoizing algorithm
bool Win_M(int a, int b, int c, vector<vector<vector<bool>>>& done, vector<vector<vector<bool>>>& solutions){
  if(done[a][b][c]) return solutions[a][b][c];
  if( a == 0 && b == 0 && c == 0) return false;
  for(int i = 1; i < a + 1; i++){
    done[a][b][c] = true;
    solutions[a][b][c] = !Win_M(a - i, b, c, done, solutions);
    if(solutions[a][b][c]) return true;
  }
  for(int i = 1; i < b + 1; i++){
    done[a][b][c] = true;
    solutions[a][b][c] = !Win_M(a, b - i, c, done, solutions);
    if(solutions[a][b][c]) return true;
  }
  for(int i = 1; i < c + 1; i++){
    done[a][b][c] = true;
    solutions[a][b][c] = !Win_M(a, b, c - i, done, solutions);
    if(solutions[a][b][c]) return true;
  }

  return false;
}

// Dynamic Programming algorithm
bool Win_DP(int a, int b, int c){
  // Initialize Cache (All locations are already false)
  vector<vector<vector<bool>>> sol(a+1,vector<vector<bool>>(b+1,vector<bool>(c+1, false)));

  // Simple solutions
  sol[0][0][0] = false;
  sol[0][0][1] = true;
  sol[0][1][0] = true;
  sol[1][0][0] = true;

  // Scan and compute
  for(int i = 0; i < a + 1; i++){
    for(int j = 0; j < b + 1; j++){
      for(int k = 0; k < c + 1; k++){
        for(int x = 1; x < a + 1; x++){
          if(i - x >= 0){
            if(sol[i][j][k]) break;
            sol[i][j][k] = !sol[i-x][j][k];
          }
        }
        for(int x = 1; x < b + 1; x++){
          if(j - x >= 0){
            if(sol[i][j][k]) break;
            sol[i][j][k] = !sol[i][j-x][k];
          }
        }
        for(int x = 1; x < c + 1; x++){
          if(k - x >= 0){
            if(sol[i][j][k]) break;
            sol[i][j][k] = !sol[i][j][k-x];
          }
        }
      }
    }
  }

  return sol[a][b][c];
}

// Call recursive algorithm
void callRecursive(int n){
  bool win = Win_R(n,n,n);
}

// Call memoizing algorithm
void callMemoizing(int n){
  vector<vector<vector<bool>>> done(n+1,vector<vector<bool>>(n+1,vector<bool>(n+1, false)));
  vector<vector<vector<bool>>> solutions(n+1,vector<vector<bool>>(n+1,vector<bool>(n+1, false)));
  bool win = Win_M(n,n,n,done,solutions);
}

// Call DP algorithm
void callDP(int n){
  bool win = Win_DP(n,n,n);
}

// Main driver
int main(){  srand(time(NULL));
  cout << "Beginning test...\n";
  ofstream fout;
  fout.open("output.txt");
  fout << "Recursive" << endl;
  fout << "-----------------------" << endl;
  fout << "n     time (in milliseconds)\n";
  fout << "=======================" << endl;
  for(int n = 2; n <= 10; n++){
    auto run_time = timer::timeFunction<decltype(callRecursive)>(callRecursive, n);
    fout << n << "     " << run_time.count() << endl;
  }

  fout << "\nMemoizing" << endl;
  fout << "-----------------------" << endl;
  fout << "n     time (in milliseconds)\n";
  fout << "=======================" << endl;
  for(int n = 2; n <= 1024; n *= 2){
    auto run_time = timer::timeFunction<decltype(callMemoizing)>(callMemoizing, n);
    fout << n << "     " << run_time.count() << endl;
  }

  fout << "\nDynamic Programming" << endl;
  fout << "-----------------------" << endl;
  fout << "n     time (in milliseconds)\n";
  fout << "=======================" << endl;
  for(int n = 2; n <= 1024; n *= 2){
    auto run_time = timer::timeFunction<decltype(callDP)>(callDP, n);
    fout << n << "     " << run_time.count() << endl;
  }

  fout.close();

  cout << "Finished!\nResults are located in output.txt\n";

  return EXIT_SUCCESS;
}

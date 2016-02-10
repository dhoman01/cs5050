#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>

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

// Generates a tuple of random integers for
// the pile sizes of Nim
tuple<int, int, int> generateProblem(int max){
  int a = rand() % max + 1;
  int b = rand() % max + 1;
  int c = rand() % max + 1;

  return make_tuple(a, b, c);
}

// Calls all three algorithms and returns
// whether the same
bool all_three_alg(int a, int b, int c){
  vector<vector<vector<bool>>> done(a+1,vector<vector<bool>>(b+1,vector<bool>(c+1, false)));
  vector<vector<vector<bool>>> solutions(a+1,vector<vector<bool>>(b+1,vector<bool>(c+1, false)));

  bool win_R = Win_R(a, b, c);
  bool win_M = Win_M(a, b, c, done, solutions);
  bool win_DP = Win_DP(a, b, c);
  return win_DP == win_M && win_M == win_R;
}

// Calls the algorithms implementing
// DP and Memoizing and returns whether
// they are the same
bool fast(int a, int b, int c){
  vector<vector<vector<bool>>> done(a+1,vector<vector<bool>>(b+1,vector<bool>(c+1, false)));
  vector<vector<vector<bool>>> solutions(a+1,vector<vector<bool>>(b+1,vector<bool>(c+1, false)));


  bool win_M = Win_M(a, b, c, done, solutions);
  bool win_DP = Win_DP(a, b, c);
  return win_M == win_DP;
}

// Main driver
int main(){
  srand(time(NULL));
  int trials = 1;
  char alg_one = 'n';
  cout << "Enter the number of trials: \n";
  cin >> trials;
  cout << "Include the recursive algorithm? y or n\n";
  cin >> alg_one;
  ofstream fout;
  fout.open("output.txt");
  switch(alg_one){
    case 'y':{
      cout << "Beginning tests...\n";
      for(int i = 0; i < trials; i++){
        tuple<int, int, int> heaps = generateProblem(5);
        int a = get<0>(heaps);
        int b = get<1>(heaps);
        int c = get<2>(heaps);
        bool result = all_three_alg(a, b, c);
        fout << "Win_R == Win_M == Win_DP with A=" << a << ", B=" << b << ", and C=" << c << " is " << result << "\n";
        if(!result){
          fout << "Something went wrong ^^^\n";
          cout << "Something went wrong! Please look in output.txt\n";
          break;
        }
      }

      break;
    }
    case 'n':{
      cout << "Beginning tests...\n";
      for(int i = 0; i < trials; i++){
        tuple<int, int, int> heaps = generateProblem(20);
        int a = get<0>(heaps);
        int b = get<1>(heaps);
        int c = get<2>(heaps);
        bool result = fast(a,b,c);
        fout << "Win_M == Win_DP with A=" << a << ", B=" << b << ", and C=" << c << " is " << result << "\n";
        if(!result){
          fout << "Something went wrong ^^^\n";
          cout << "Something went wrong! Please look in output.txt\n";
          break;
        }
      }

      break;
    }
  }

  cout << "Finished!\nResults are located in output.txt\n";

  fout.close();

  return 0;
}

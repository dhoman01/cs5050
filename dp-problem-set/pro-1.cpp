#include <iostream>
#include <vector>
#include <cstdlib>

double A[] = {-30,-10,2,4,-50,50,2};

double sum(int i, int j)
{
  double total = 0;
  for(int k = i; k <= j; k++)
  {
    total += A[k];
  }

  return total;
}

double MVC(int i, int j)
{
  std::cout << "i, j " << i << ", " << j << std::endl;
  if(i == j) return A[i];
  double a = MVC(i+1,j);
  double b = MVC(i,j-1);
  double c = sum(i,j);
  std::cout << "a, b, and c: " << a << ", " << b << ", " << c << std::endl;
  double m = std::max(a,std::max(b,c));
  std::cout << "max: " << m << std::endl;
  return m;
}

double MVC_DP(int i, int j)
{
  std::vector<std::vector<double>> MVC (i+1, std::vector<double>(j+1,0));
  for(int a = 0; a <= i; a++)
  {
    for(int b = 0; b <= j; b++)
    {
      if(a == b) MVC[a][b] = a;
    }
  }
}

int main()
{
  std::cout << "MVCS R: " << MVC(0, 6) << std::endl;

  return EXIT_SUCCESS;
}

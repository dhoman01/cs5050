#include <iostream>
#include <cstdlib>

int v(int n){
  switch(n)
  {
    case 1:
      return 1;
      break;
    case 2:
      return 5;
      break;
    case 3:
      return 10;
      break;
    case 4:
      return 25;
      break;
    default:
      return 0;
      break;
  }
}

int change(int n, int C)
{
  if(n==1) return C;
  if(C<=0) return 0;
  return std::min(change(n-1,C),change(n,C-v(n)) + 1);
}

int main()
{
  std::cout << "change(4,167) " << change(4,167) << std::endl;

  return EXIT_SUCCESS;
}

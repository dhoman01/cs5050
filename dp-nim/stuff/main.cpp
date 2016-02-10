#include <iostream>

using namespace std;

int main(){
  int width, height, n;
  cout << "Enter width and height and n: " << endl;
  cin >> width >> height >> n;

  int minX = 0, maxX = width / n, minY = 0, maxY = height / n;

  for(int i = 0; i < n; i++){
    cout << "minX maxY minY maxY: " << minX << " " << maxX << " " << minY << " " << maxY << endl;
    minX = 1 + maxX;
    minY = 1 + maxY;
    maxX += width / n;
    maxY += height / n;
  }

  return 0;
}

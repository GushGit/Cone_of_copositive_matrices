#include <bits/stdc++.h>
#include "is_copositive.h"

#define ld long double

using namespace std;

int cliqueNumber(vector<vector<int>> g) {
  int n = g.size();

  int cliqueSize = 1;
  while(true) {
    
    ld currAlpha = (ld) (cliqueSize - 1) / cliqueSize;
    ld nextAlpha = (ld) cliqueSize / (cliqueSize + 1);
    ld alpha = (currAlpha + nextAlpha) / 2;

    vector<vector<ld>> v(n, vector<ld> (n));
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        v[i][j] = -g[i][j] + alpha;
      }
    }
    
    bool copositive = isCopositive(v);
    cout << cliqueSize << endl;
    if(copositive) break;
    cliqueSize++;
  }
  
  return cliqueSize;
}

int main() {

  cout << "Input file: ";
  string fileName;
  cin >> fileName;

  ifstream fin(fileName);

  int t;
  fin >> t;

  for(int i = 1; i <= t; i++) {
    int n;
    fin >> n;

    vector<vector<int>> v(n, vector<int> (n));
    for(auto &x : v) {
      for(auto &y : x) {
        fin >> y;
      }
    }

    int expected;
    fin >> expected;
    if(expected != cliqueNumber(v)) {
      cout << "WRONG ANSWER ON TEST " << i << endl;
    } else cout << "PASSED TEST " << i << endl;
  }

}



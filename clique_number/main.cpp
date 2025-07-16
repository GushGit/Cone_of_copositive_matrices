#include <bits/stdc++.h>
#include "clique_finder.h"

#define ld long double

using namespace std;

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

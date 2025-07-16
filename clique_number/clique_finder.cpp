#include <bits/stdc++.h>
#include "../copositive_checker.h"

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
        if(copositive) break;
        cliqueSize++;
    }
    
    return cliqueSize;
}

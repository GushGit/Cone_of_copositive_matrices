#include <bits/stdc++.h>
#include "../copositive_checker.h"
#include "../clique_number/clique_finder.h"

#define ld long double

using namespace std;

string conf(vector<int> vals) {
    string ans = "";
    int n = vals.size();

    for(int i = 0; i < n - 1; i++) {
        ans += to_string(vals[i]) + "_";
    } ans += to_string(vals[n - 1]);

    return ans;
}

int main() {
    string iconf;
    cin >> iconf;
    ifstream fin("../measurement_interface/input/" + iconf);

    string commentary;
    getline(fin, commentary);

    int type;
    fin >> type;

    int t;
    fin >> t;

    {
        fin >> checkSubmatricesUpTo; // 1, 2, 3
        fin >> randomVectors; // non-neg integer
        fin >> checkSum; // 0, 1
        fin >> checkInSP; // 0, 1
        fin >> checkInSPPN; // 0, 1
        fin >> checkSubtractInSP; // 0, 1
    }

    string configuration = conf({
        checkSubmatricesUpTo, 
        randomVectors, 
        checkSum,
        checkInSP,
        checkInSPPN,
        checkSubtractInSP,
    });

    cout << iconf << '\n';
    ofstream fout("../measurement_interface/output/" + iconf);

    fout << commentary << '\n';

    long long ns = 0;
    for(int i = 1; i <= t; i++) {
        int n;
        fin >> n;

        vector<vector<ld>> v(n, vector<ld> (n));
        for(auto &x : v) {
            for(auto &y : x) {
                fin >> y;
            }
        }

        auto t1 = chrono::high_resolution_clock::now();
        if(type == 0) {
            isCopositive(v);
        } else {
            vector<vector<int>> am(n, vector<int>(n));
            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++) {
                    am[i][j] = (int) v[i][j];
                }
            }
            cliqueNumber(am);
        }
        auto t2 = chrono::high_resolution_clock::now();

        auto delta = chrono::duration_cast<chrono::nanoseconds> (t2 - t1).count();
        ns += delta;

        fout << delta << '\n';
    }

    fout << ns << '\n';
    fout << matricesChecked << '\n';
}

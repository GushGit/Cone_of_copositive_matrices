#include <bits/stdc++.h>
#include "../copositive_checker.h"

#define ld long double

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    {
        cin >> checkSubmatricesUpTo; // 1, 2, 3
        cin >> randomVectors; // non-neg integer
        cin >> checkSum; // 0, 1
        cin >> checkInSP; // 0, 1
        cin >> checkInSPPN; // 0, 1
        cin >> checkSubtractInSP; // 0, 1
    }

    int t;
    cin >> t;

    long long ns = 0;
    for(int i = 1; i <= t; i++) {
        int n;
        cin >> n;

        vector<vector<ld>> v(n, vector<ld> (n));
        for(auto &x : v) {
            for(auto &y : x) {
                cin >> y;
            }
        }

        auto t1 = chrono::high_resolution_clock::now();
        isCopositive(v);
        auto t2 = chrono::high_resolution_clock::now();
        ns += chrono::duration_cast<chrono::nanoseconds> (t2 - t1).count();
    }

    cout << ns << endl;
    cout << matricesChecked << endl;
}

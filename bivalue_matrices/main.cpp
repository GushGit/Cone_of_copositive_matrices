#include <bits/stdc++.h>

#define ld long double

using namespace std;

bool hasKClique(vector<vector<int>> g, int k) {
    int n = g.size();

    if(k > n) return false;
    if(k > 1 && !hasKClique(g, k - 1)) return false;

    vector<int> v(n);
    for(int i = n - k; i < n; i++) {
        v[i] = 1;
    }

    while(true) {
        vector<int> curr;
        for(int i = 0; i < n; i++) {
            if(v[i]) curr.push_back(i);
        }

        bool ok = true;
        for(int i = 0; i < k; i++) {
            for(int j = i + 1; j < k; j++) {
                if(!g[curr[i]][curr[j]]) {
                    ok = false;
                    break;
                }
            }
            if(!ok) break;
        }
        if(ok) return true;

        if(!next_permutation(v.begin(), v.end())) break;
    }

    return false;
}

bool check(vector<vector<ld>> v) {
    int n = v.size();
    if(n == 0) return true;

    vector<ld> vals;
    for(auto &x : v) {
        for(auto y : x) {
            if(vals.size() == 2 && (vals[0] != y && vals[1] != y)) return false;
            if(vals.size() != 0 && (vals.size() != 1 || vals[0] == y)) continue;
            vals.push_back(y);
            sort(vals.begin(), vals.end());
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = i; j < n; j++) {
            if(v[i][j] != v[j][i]) return false;
        }
    }

    if(vals[0] >= 0) return true;
    if(vals.back() < 0) return false;

    ld alpha = vals.back() / (vals.back() - vals[0]);

    vector<vector<int>> g(n, vector<int> (n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(v[i][j] < 0) g[i][j] = 1;
        }
    }

    int k = (1 / (1 - alpha)) + 1;
    return !hasKClique(g, k);
}

int main() {
    
    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<vector<ld>> v(n, vector<ld> (n));
        for(auto &x : v) {
            for(auto &y : x)
                cin >> y;
        }
        cout << check(v) << endl;
    }
}

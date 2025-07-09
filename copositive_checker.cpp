#include <bits/stdc++.h>

#define ld long double

using namespace std;

const ld EPSILON = 1e-6;

vector<vector<ld>> mul(vector<vector<ld>> a, vector<vector<ld>> b) {
    int n = a.size();
    int m = b.size();
    int r = b[0].size();

    vector<vector<ld>> c(n, vector<ld> (r));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < r; j++) {
            for(int k = 0; k < m; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return c;
}

vector<ld> mul(vector<vector<ld>> a, vector<ld> b) {
    int n = a.size();
    int m = b.size();

    vector<ld> c(n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            c[i] += a[i][j] * b[j];
        }
    }

    return c;
}

vector<vector<ld>> mul(vector<vector<ld>> a, ld b) {
    for(auto &x : a) {
        for(auto &y : x) {
            y *= b;
        }
    }

    return a;
}

vector<vector<ld>> add(vector<vector<ld>> a, vector<vector<ld>> b) {
    int n = a.size();
    int m = a[0].size();

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            a[i][j] += b[i][j];
        }
    }

    return a;
}

vector<vector<ld>> pow(vector<vector<ld>> v, int p) {
    if(p == 1) return v;
    if(p & 1) return mul(v, pow(v, p - 1));
    vector<vector<ld>> g = pow(v, p / 2);
    return mul(g, g);
}

ld length(vector<ld> v) {
    ld sum = 0;
    for(auto x : v) {
        sum += x * x;
    }
    return sqrt(sum);
}

vector<ld> normalized(vector<ld> v) {
    ld len = length(v);
    for(auto &x : v) {
        x /= len;
    }
    return v;
}

ld absoluteLargestEigenvalue(vector<vector<ld>> v) {
    int n = v.size();

    vector<ld> x0(n, 1);

    int m = 1 << 4;
    vector<ld> xm = normalized(mul(pow(v, m), x0));
    
    xm = mul(v, xm);
    return length(xm);
}

ld largestEigenvalue(vector<vector<ld>> v) {
    int n = v.size();

    ld absoluteLargest = absoluteLargestEigenvalue(v);
    for(int i = 0; i < n; i++) {
        v[i][i] += absoluteLargest;
    }

    return absoluteLargestEigenvalue(v) - absoluteLargest;
}

bool failsValues(vector<vector<ld>> v, int lastChange) {
    // Check non-negativity for vector of 1s
    ld sum = 0;
    for(auto &x : v) {
        for(auto y : x) {
            sum += y;
        }
    }
    if(sum < 0) return true;

    // TODO
    // Check non-negativity for few random vector maybe idk
    return false;
}

// Check if every 1x1 submatrix is in COP
bool failsCOP1(vector<vector<ld>> v, int lastChange) {
    int n = v.size();

    for(int i = 0; i < n; i++) {
        //if(lastChange != -1 && i != lastChange) continue;
        if(v[i][i] < 0) return true;
    }

    return false;
}

// Check if every 2x2 submatrix is in COP
bool failsCOP2(vector<vector<ld>> v, int lastChange) {
    if(failsCOP1(v, lastChange)) return true;
    int n = v.size();

    for(int i = 0; i < n; i++) {
        if(lastChange != -1 && i != lastChange) continue;
        for(int j = i + 1; j < n; j++) {
            if(v[i][j] >= 0) continue;
            if(v[i][j] * v[i][j] > v[i][i] * v[j][j]) return true;
        }
    }

    return false;
}

// Check if every 3x3 submatrix is in COP
bool failsCOP3(vector<vector<ld>> v, int lastChange) {
    if(failsCOP2(v, lastChange)) return true;
    int n = v.size();

    for(int i = 0; i < n; i++) {
        if(lastChange != -1 && i != lastChange) continue;
        for(int j = i + 1; j < n; j++) {
            for(int k = j + 1; k < n; k++) {
                ld sum = 0;
                sum += acos(-v[i][j] / sqrt(v[i][j]) / sqrt(v[j][j]));
                sum += acos(-v[i][k] / sqrt(v[i][i]) / sqrt(v[k][k]));
                sum += acos(-v[j][k] / sqrt(v[j][j]) / sqrt(v[k][k]));
                if(sum < acos(-1)) return true;
            }
        }
    }

    return false;
}

bool failsNecessary(vector<vector<ld>> v, int lastChange) {
    if(failsValues(v, lastChange)) return true;
    if(failsCOP3(v, lastChange)) return true;
    // checking failsCOP2 instead of failsCOP3 seems to be better in some clique testcase?
    // checking failsCOP1 is faster?
    // checking ONLY failsCOP1 is even faster?!
    // interesting
    return false;
}

bool passesDominantDiagonals(vector<vector<ld>> v) {
    int n = v.size();

    for(int i = 0; i < n; i++) {
        ld negativeSum = 0;
        for(int j = 0; j < n; j++) {
            if(v[i][j] > 0 || i == j) continue;
            negativeSum += v[i][j];
        }
        if(v[i][i] + negativeSum < -EPSILON) return false;
    }

    return true;
}

bool passesInSP(vector<vector<ld>> v) {
    // TODO gives false positives
    if(1) return false;
    int n = v.size();

    vector<vector<ld>> f(n, vector<ld> (n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            f[min(i, j)][max(i, j)] += v[i][j];
        }
    }

    for(int i = n - 1; i > 0; i--) {
        vector<ld> sol(i);
        for(int j = 0; j < i; j++) {
            sol[j] = -f[j][i] / f[i][i] / 2;
        }
        for(int j = 0; j < i; j++) {
            for(int k = 0; k < i; k++) {
                f[min(j, k)][max(j, k)] += f[i][i] * sol[j] * sol[k];
                f[min(j, k)][max(j, k)] += f[j][i] * sol[k];
            }
        }
    }

    return f[0][0] >= -EPSILON;
}

// Not sure what exactly it passes
bool _passesInSP(vector<vector<ld>> v) {
    // TODO for now it only slows down the algo (zero return true rate)
    // (isnt it impossible to hit true?)
    if(1) return false;

    int n = v.size();

    vector<vector<ld>> invq(n, vector<ld> (n));
    // TODO invq (Q^-1) = ...
    for(int i = 0; i < n; i++) {
        invq[i][i] -= 1 / v[i][i];
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            invq[i][j] += 1 / ((n - 1) * sqrt(v[i][i]) * sqrt(v[j][j]));
        }
    }

    vector<vector<ld>> g = mul(invq, v);
    ld largest = largestEigenvalue(g);
    if(largest < -EPSILON) return false;

    vector<vector<ld>> q(n, vector<ld> (n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) continue;
            q[i][j] = sqrt(v[i][i]) * sqrt(v[j][j]);
        }
    }

    vector<vector<ld>> h = add(v, mul(q, -largest));
    for(auto &x : h) {
        for(auto y : x) {
            if(y < -EPSILON) return false;
        }
    }

    return true;
}

bool passesSufficient(vector<vector<ld>> v) {
    if(v.size() == 0) return true;
    if(v.size() == 1) return !failsCOP1(v, -1);
    if(v.size() == 2) return !failsCOP2(v, -1);
    if(v.size() == 3) return !failsCOP3(v, -1);
    if(passesDominantDiagonals(v)) return true;
    if(passesInSP(v)) return true;

    return false;
}

bool isCopositive(vector<vector<ld>> g) {
    queue<pair<vector<vector<ld>>, int>> q;
    q.push({g, -1});

    while(q.size()) {
        vector<vector<ld>> v = q.front().first;
        int prev = q.front().second;
        q.pop();
        int n = v.size();

        // Reducing matrix size if possible
        vector<int> req(n);
        iota(req.begin(), req.end(), 0);
        vector<ld> negSums(n);
        {
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    negSums[i] += v[i][j] * (v[i][j] < 0);
                }
                if(negSums[i] == 0) req.erase(find(req.begin(), req.end(), i));
            }
        }

        n = req.size();
        vector<vector<ld>> nv(req.size(), vector<ld> (req.size()));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                nv[i][j] = v[req[i]][req[j]];
            }
        }
        v.swap(nv);

        if(failsNecessary(v, prev)) return false;
        if(passesSufficient(v)) continue;

        // Choosing edge to split
        int row = 0;
        for(int i = 0; i < n; i++) {
            if(v[i][i] + negSums[req[i]] < v[row][row] + negSums[req[row]]) {
                row = i;
            }
        }

        int column = 0;
        for(int i = 0; i < n; i++) {
            if(v[row][i] < v[row][column]) {
                column = i;
            }
        }

        // Splitting
        ld alpha = (v[column][column] - v[row][column]) / (v[row][row] - v[row][column] * 2 + v[column][column]);
        for(int _ = 0; _ < 2; _++) {
            vector<vector<ld>> next(n, vector<ld> (n));
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    next[i][j] = v[i][j];
                }
            }

            for(int i = 0; i < n; i++) {
                next[row][i] = next[row][i] * alpha + next[column][i] * (1 - alpha);
            }
            for(int i = 0; i < n; i++) {
                next[i][row] = next[i][row] * alpha + next[i][column] * (1 - alpha);
            }

            q.push({next, row});

            swap(row, column);
            alpha = 1 - alpha;
        }
    }

    return true;
}

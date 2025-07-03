#include <bits/stdc++.h>

#define ld long double

using namespace std;

bool isCopositive(vector<vector<ld>> g) {

  queue<pair<vector<vector<ld>>, int>> q;
  q.push({g, -1});

  while(q.size()) {
    vector<vector<ld>> v = q.front().first;
    int prev = q.front().second;
    q.pop();
    int n = v.size();
    
    {
      ld sum = 0;
      for(auto &x : v) {
        for(auto y : x) {
          sum += y;
        }
      }
      if(sum < 0) return false;
    }

    {
      for(int i = 0; i < n; i++) {
        if(v[i][i] < 0) return false;
      }
    }

    {
      for(int i = 0; i < n; i++) {
        if(prev != -1 && i != prev) continue;
        for(int j = i + 1; j < n; j++) {
          if(v[i][j] >= 0) continue;
          if(v[i][j] * v[i][j] > v[i][i] * v[j][j]) return false;
        }
      }
    }

    {
      if(abs(rand()) % 16 == 0) {
        for(int i = 0; i < n; i++) {
          if(prev != -1 && i != prev) continue;
          for(int j = i + 1; j < n; j++) {
            for(int k = j + 1; k < n; k++) {
              ld sum = 0;
              sum += acos(-v[i][j] / sqrt(v[i][j]) / sqrt(v[j][j]));
              sum += acos(-v[i][k] / sqrt(v[i][i]) / sqrt(v[k][k]));
              sum += acos(-v[j][k] / sqrt(v[j][j]) / sqrt(v[k][k]));
              if(sum < acos(-1)) return false;
            }
          }
        }
      }
    }

    vector<int> req(n);
    iota(req.begin(), req.end(), 0);
    vector<ld> negSums(n);
    {
    	int failed = 0;
    	for(int i = 0; i < n; i++) {
    		for(int j = 0; j < n; j++) {
    			negSums[i] += v[i][j] * (v[i][j] < 0);
    		}
    		if(v[i][i] + negSums[i] < 0) failed++;
    		if(negSums[i] == 0) req.erase(find(req.begin(), req.end(), i));
    	}
    	if(!failed) continue;
    }

    int m = req.size();

    int r = 0;
		for(int i = 0; i < m; i++) {
			if(v[req[i]][req[i]] + negSums[req[i]] < v[req[r]][req[r]] + negSums[req[r]]) {
				r = i;
			}
		}

		int c = 0;
		for(int i = 0; i < m; i++) {
			if(v[req[r]][req[i]] < v[req[r]][req[c]]) {
				c = i;
			}
		}

		ld alpha = (v[req[c]][req[c]] - v[req[r]][req[c]]) / (v[req[r]][req[r]] - v[req[r]][req[c]] * 2 + v[req[c]][req[c]]);
		for(int _ = 0; _ < 2; _++) {
			vector<vector<ld>> next(m, vector<ld> (m));
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < m; j++) {
					next[i][j] = v[req[i]][req[j]];
				}
			}

			for(int i = 0; i < m; i++) {
				next[r][i] = next[r][i] * alpha + next[c][i] * (1 - alpha);
			}
			for(int i = 0; i < m; i++) {
				next[i][r] = next[i][r] * alpha + next[i][c] * (1 - alpha);
			}
			
			q.push({next, r});

			swap(r, c);
			alpha = 1 - alpha;
		}
  }

	return true;
}



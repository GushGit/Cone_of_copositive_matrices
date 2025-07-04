#include <bits/stdc++.h>

#define ld long double

using namespace std;

int diagonalZeros = 0;
int isCopositiveCalls = 0;

struct Matrix {
	int size;
	vector<vector<ld>> v;
	
	ld sum;
	vector<ld> negSums;

	bool failed;
	
	Matrix(int size) {
		this->size = size;
		v = vector<vector<ld>> (size, vector<ld> (size));
		
		sum = 0;
		negSums = vector<ld> (size);

		failed = false;
	}

	vector<pair<int, int>> changes;

	tuple<int, int, ld> set(int i, int j, ld val) {
		if(i == j && val == 0) diagonalZeros++;
		sum -= v[i][j];
		sum += val;
		if(i != j && v[i][j] < 0) negSums[i] -= v[i][j];
		ld prev = v[i][j];
		v[i][j] = val;
		if(i != j && v[i][j] < 0) negSums[i] += v[i][j];

		if(i == j && v[i][i] >= 0) {
			for(int k = 0; k < size; k++) {
				if(i == k) continue;
				changes.push_back({i, k});
			}
		} else changes.push_back({i, j});

		return {i, j, prev};
	}

	void changesUpdate() {
		for(auto [i, j] : changes) {
			if(i == j || v[i][j] >= 0) continue;
			if(v[i][j] * v[i][j] > v[i][i] * v[j][j]) failed = true;
		}
		changes.clear();
	}

	void fullUpdate() {
		sum = 0;
		for(auto &x : v) {
			for(auto y : x) {
				sum += y;
			}
		}
		negSums = vector<ld> (size, 0);
		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				if(i == j || v[i][j] >= 0) continue;
				negSums[i] += v[i][j];
			}
		}
		failed = false;
		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				if(i == j || v[i][j] >= 0) continue;
				if(v[i][j] * v[i][j] > v[i][i] * v[j][j]) failed = true;
			}
		}
	}

	vector<ld> &operator[](int i) {
		return v[i];
	}

};

bool isCopositive(Matrix &v, int depth, int &maxDepth, int &timer) {
	if(isCopositiveCalls % 1000 == 0) cout << isCopositiveCalls << " " << depth << " " << maxDepth << " " << v.size << endl;

	isCopositiveCalls++;

	int n = v.size;

	maxDepth = max(maxDepth, depth);

	if(timer % n == 0) v.fullUpdate();
	timer++;

	{
		bool ok = true;
		for(int i = 0; i < n; i++) {
			if(v[i][i] == 0) ok = false;
		}
		if(ok && 0) {
			for(int i = 0; i < n; i++) {
				if(v[i][i] != 1) {
					ld sq = 1.0 / sqrtl(v[i][i]);
					for(int j = 0; j < n; j++) {
						if(i == j) continue;
						v.set(i, j, v[i][j] * sq);
						v.set(j, i, v[j][i] * sq);
					}
					v.set(i, i, 1);
				}
			}
		}
	}

	// Необходимые условия
	{
		if(v.failed) return false;
		if(v.sum < 0) return false;
		for(int i = 0; i < n; i++) {
			if(v[i][i] < 0) return false;
		}
	}

	Matrix newV(0);
	// Достаточное условие
	{
		int cnt = 0;
		int mini = 0;
		vector<int> removable;
		for(int i = 0; i < n; i++) {
			if(v.negSums[i] == 0) removable.push_back(i);
			if(v[i][i] + v.negSums[i] < v[mini][mini] + v.negSums[mini]) {
				mini = i;
			}
			if(v[i][i] + v.negSums[i] < 0) cnt++;
		}
		if(v[mini][mini] + v.negSums[mini] >= 0) return true;
		else if(cnt == 1) {
			vector<tuple<int, int, ld>> changes;
			ld mul = -v.negSums[mini] / v[mini][mini];
			for(int i = 0; i < n; i++) {
				changes.push_back(v.set(i, mini, v[i][mini] * mul));
				changes.push_back(v.set(mini, i, v[mini][i] * mul));
			}
			int minj = 0;
			for(int i = 1; i < n; i++) {
				if(v[i][i] + v.negSums[i] < v[minj][minj] + v.negSums[minj]) {
					minj = i;
				}
			}
			reverse(changes.begin(), changes.end());
			for(auto [i, j, val] : changes) {
				v.set(i, j, val);
			}
			if(v[minj][minj] + v.negSums[minj] >= 0) return true;
		}
		if(removable.size()) {
			vector<int> g(n);
			iota(g.begin(), g.end(), 0);
			for(auto x : removable) {
				g.erase(find(g.begin(), g.end(), x));
			}
			int m = g.size();
			newV = Matrix(g.size());
			for(int i = 0; i < m; i++) {
				for(int j = 0; j < m; j++) {
					newV.set(i, j, v[g[i]][g[j]]);
				}
			}
		}
	}

	int r = 0;
	for(int i = 1; i < n; i++) {
		if(v[i][i] + v.negSums[i] < v[r][r] + v.negSums[r]) {
			r = i;
		}
	}

	int c = 0;
	for(int i = 1; i < n; i++) {
		if(v[r][i] < v[r][c]) {
			c = i;
		}
	}

	ld alpha = (v[c][c] - v[r][c]) / (v[r][r] - v[r][c] * 2 + v[c][c]);
	for(int _ = 0; _ < 2; _++) {
		vector<tuple<int, int, ld>> changes(n * 2);
		for(int i = 0; i < n; i++) {
			changes[i] = v.set(r, i, v[r][i] * alpha + v[c][i] * (1 - alpha));
		}
		for(int i = 0; i < n; i++) {
			changes[n + i] = v.set(i, r, v[i][r] * alpha + v[i][c] * (1 - alpha));
		}
		v.changesUpdate();
		if(!isCopositive((newV.size ? newV : v), depth + 1, maxDepth, timer)) return false;

		reverse(changes.begin(), changes.end());
		for(auto [i, j, val] : changes) {
			v.set(i, j, val);
		}

		swap(r, c);
		alpha = 1 - alpha;
	}

	return true;
}

bool solveForCopositive(ifstream &fin) {
	int n;
	fin >> n;

	Matrix v(n);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			ld x;
			fin >> x;
			v.set(i, j, x);
		}
	}

	int maxDepth = 0;
	int timer = 0;
	bool copositive = isCopositive(v, 0, maxDepth, timer);
	cout << "Copositive: " << (copositive ? "YES" : "NO") << endl;
	cout << "Max Search Depth: " << maxDepth << endl;

	string s;
	fin >> s;
	bool expected = (s == "YES");

	return (copositive == expected);
}

bool isCopositiveForCliqueSize(vector<vector<int>> &g, int cliqueSize) {
	int n = g.size();
	Matrix v(n);
	ld currAlpha = (ld) (cliqueSize - 1) / cliqueSize;
	ld nextAlpha = (ld) cliqueSize / (cliqueSize + 1);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			ld alpha = (currAlpha + nextAlpha) / 2;
			v.set(i, j, alpha - g[i][j]);
		}
	}
	int maxDepth = 0;
	int timer = 0;
	int temp = isCopositiveCalls;
	bool copositive = isCopositive(v, 0, maxDepth, timer);
	return copositive;
}

int findCliqueNumber(vector<vector<int>> &g) {
	int n = g.size();

	for(int i = n; i >= 1; i--) {
		int temp = isCopositiveCalls;
		bool copositive = isCopositiveForCliqueSize(g, i);
		if(copositive) {
			cout << "YES ";
		} else cout << "NO ";
		cout << isCopositiveCalls - temp << endl;
	}

	int l = 1, r = n;
	while(l <= r) {
		int mid = (l + r) / 2;
		int temp = isCopositiveCalls;
		bool copositive = isCopositiveForCliqueSize(g, mid);
		cout << mid << endl;
		if(copositive) r = mid - 1;
		else l = mid + 1;
	}

	return l;
}

bool solveForClique(ifstream &fin) {
	int n;
	fin >> n;

	vector<vector<int>> g(n, vector<int> (n));
	for(auto &x : g) {
		for(auto &y : x) {
			fin >> y;
		}
	}

	int cliqueNumber = findCliqueNumber(g);
	cout << "Clique number: " << cliqueNumber << endl;

	int expected;
	fin >> expected;

	return (cliqueNumber == expected);
}

enum Mode {
	IS_COPOSITIVE,
	CLIQUE_NUMBER
};

int32_t main() {
	
	cout << "Input file name: ";
	string inputName;
	cin >> inputName;
	cout << endl;
	ifstream fin(inputName);

	string modeString;
	fin >> modeString;

	Mode mode;
	if(modeString == "IS_COPOSITIVE") mode = IS_COPOSITIVE;
	if(modeString == "CLIQUE_NUMBER") mode = CLIQUE_NUMBER;
	
	vector<int> unexpected;

	int t = 1;
	fin >> t;
	for(int i = 1; i <= t; i++) {
		cout << "# Test case " << i << endl;
		bool passed = true;
		if(mode == IS_COPOSITIVE) passed = solveForCopositive(fin);
		if(mode == CLIQUE_NUMBER) passed = solveForClique(fin);
		if(!passed) unexpected.push_back(i);
		cout << "---" << endl << endl;
	}

	if(unexpected.size()) {
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << endl;
		cout << "UNEXPECTED RESULT ON TESTS: ";
		for(int i = 0; i < unexpected.size(); i++) {
			cout << unexpected[i];
			cout << ((i == unexpected.size() - 1) ? "\n" : ", ");
		}
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	}

	cout << "Diagonal zeros during all tests: " << diagonalZeros << endl;
	cout << "isCopositive calls during all tests: " << isCopositiveCalls << endl;
}

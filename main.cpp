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
	isCopositiveCalls++;

	int n = v.size;

	maxDepth = max(maxDepth, depth);

	if(timer % n == 0) v.fullUpdate();
	timer++;

	// Необходимые условия
	{
		if(v.failed) return false;
		if(v.sum < 0) return false;
		for(int i = 0; i < n; i++) {
			if(v[i][i] < 0) return false;
		}
	}

	// Достаточное условие
	{
		bool ok = true;
		for(int i = 0; i < n; i++) {
			if(v[i][i] < -v.negSums[i]) ok = false;  
		}
		if(ok) return true;
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

	ld alpha = 0.5;
	for(int _ = 0; _ < 2; _++) {
		vector<tuple<int, int, ld>> changes(n * 2);
		for(int i = 0; i < n; i++) {
			changes[i] = v.set(r, i, v[r][i] * alpha + v[c][i] * (1 - alpha));
		}
		for(int i = 0; i < n; i++) {
			changes[n + i] = v.set(i, r, v[i][r] * alpha + v[i][c] * (1 - alpha));
		}
		v.changesUpdate();
		if(!isCopositive(v, depth + 1, maxDepth, timer)) return false;

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

int findCliqueNumber(vector<vector<int>> &g) {
	int n = g.size();

	int l = 1, r = n;
	while(l <= r) {
		int mid = (l + r) / 2;
		Matrix v(n);
		ld currAlpha = (ld) (mid - 1) / mid;
		ld nextAlpha = (ld) mid / (mid + 1);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				v.set(i, j, (currAlpha + nextAlpha) / 2 - g[i][j]);
			}
		}
		int maxDepth = 0;
		int timer = 0;
		bool copositive = isCopositive(v, 0, maxDepth, timer);
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

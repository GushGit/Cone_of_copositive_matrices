#include <bits/stdc++.h>

#define ld long double

using namespace std;

struct Matrix {
	int size;
	vector<vector<ld>> v;
	
	ld sum;
	vector<ld> negSums;
	
	Matrix(int size) {
		this->size = size;
		v = vector<vector<ld>> (size, vector<ld> (size));
		
		sum = 0;
		negSums = vector<ld> (size);
	}

	tuple<int, int, ld> set(int i, int j, ld val) {
		sum -= v[i][j];
		sum += val;
		if(i != j && v[i][j] < 0) negSums[i] -= v[i][j];
		ld prev = v[i][j];
		v[i][j] = val;
		if(i != j && v[i][j] < 0) negSums[i] += v[i][j];
		return {i, j, prev};
	}

	vector<ld> &operator[](int i) {
		return v[i];
	}

};

bool isCopositive(Matrix &v, int depth, int &maxDepth) {
	maxDepth = max(maxDepth, depth);
	if(v.sum < 0) return false;
	//if(depth > 128) return true;
	//cout << depth << " " << v[0][0] << endl;

	int n = v.size;

	// Необходимое условие
	{
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
		if(!isCopositive(v, depth + 1, maxDepth)) return false;

		reverse(changes.begin(), changes.end());
		for(auto [i, j, val] : changes) {
			v.set(i, j, val);
		}

		swap(r, c);
		alpha = 1 - alpha;
	}

	return true;
}

bool solve(ifstream &fin) {
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
	bool copositive = isCopositive(v, 0, maxDepth);
	cout << "Copositive: " << (copositive ? "YES" : "NO") << endl;
	cout << "Max Search Depth: " << maxDepth << endl;

	string s;
	fin >> s;
	bool expected = (s == "YES");

	return (expected == copositive);
}

int32_t main() {
	
	ifstream fin("input.txt");
	
	vector<int> unexpected;

	int t = 1;
	fin >> t;
	for(int i = 1; i <= t; i++) {
		cout << "# Test case " << i << endl;
		cout << "---" << endl;
		if(!solve(fin)) unexpected.push_back(i);
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
	
}

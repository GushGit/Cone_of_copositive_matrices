#include <bits/stdc++.h>
#include "copositive_checker.h"

using namespace std;

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

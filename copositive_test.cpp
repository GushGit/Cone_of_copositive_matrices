#include <bits/stdc++.h>
#include "is_copositive.h"

#define ld long double

using namespace std;

int main() {

	cout << "Input file: ";
	string fileName;
	cin >> fileName;

	ifstream fin(fileName);

	int t;
	fin >> t;

	for(int i = 1; i <= t; i++) {
		int n;
		fin >> n;

		vector<vector<ld>> v(n, vector<ld> (n));
		for(auto &x : v) {
			for(auto &y : x) {
				fin >> y;
			}
		}

		string s;
		fin >> s;
		bool expected = (s == "YES");
		if(expected != isCopositive(v)) {
			cout << "WRONG ANSWER ON TEST " << i << endl;
		} else cout << "PASSED TEST " << i << endl;
	}

}

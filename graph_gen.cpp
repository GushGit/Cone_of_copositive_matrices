#include <bits/stdc++.h>
#include <cstdlib>

#define int long long int

using namespace std;

void printm(vector<vector<int>> &m, ofstream &fout) {
	fout << m.size() << endl;
	for (int i = 0; i < m.size(); ++i)
	{
		for (int j = 0; j < m.size(); ++j)
		{
			fout << m[i][j] << " ";
		}
		fout << endl;
	}
	fout << endl;
}
void printm(vector<vector<int>> &m) {
	cout << m.size() << endl;
	for (int i = 0; i < m.size(); ++i)
	{
		for (int j = 0; j < m.size(); ++j)
		{
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void generate_graph(ifstream &fin, ofstream &fout) {
	int n, k;
	vector<int> cl_sizes;
	fin >> n >> k;
	for (int i = 0; i < k; ++i)
	{
		int val;
		fin >> val;
		cl_sizes.push_back(val);
	}

	vector<vector<int>> m;
	m = vector<vector<int>> (n, vector<int> (n));

	for(auto k : cl_sizes) {
		vector<int> cl_idx = vector<int> (k, -1);
		//cout << ">";
		for (int i = 0; i < k; ++i)
		{
			int idx = rand() % n;
			while(find(begin(cl_idx), end(cl_idx), idx) != end(cl_idx)) {
				idx = rand() % n;
			}
			cl_idx[i] = idx;
			//cout << idx << ' ';
		}
		//cout << endl;

		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				m[cl_idx[i]][cl_idx[j]] = 1;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		m[i][i] = 0;
	}

	//printm(m);
	printm(m, fout);
}


int incr(int i, int k) {
	int t = i;
	int pos = 0;
	while(t > 0) {
		if(t % 2 == 1) {
			break;
		}
		t /= 2;
		pos++;
	}
	i += (int) pow(2, pos);

	int cnt = 0;
	t = i;
	while(t > 0) {
		cnt += t % 2;
		t /= 2;
	}

	return i + (int) pow(2, k - cnt) - 1;
}	

string decToBinary(int n) {
    string bin = "";
    while (n > 0) {
		int bit = n % 2;
      	bin.push_back('0' + bit);
        n /= 2;
    }
    
	reverse(bin.begin(), bin.end());
    return bin;
}

bool find_k_clique(ifstream &fin, int k) {
	int n;
	int val;
	fin >> n;
	vector<vector<int>> m(n);

	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			fin >> val;
			m[i].push_back(val);
		}
	}

	for(int mask = (int) pow(2, k) - 1; mask < (int) pow(2, n) - 1; mask = incr(mask, k)) {
		if(mask % 1024 == 0) {
			string binmask = decToBinary(mask);
			binmask.insert(0, n-binmask.size(), '0');
			cout << binmask << endl;
		}

		vector<int> idx;
		int t_mask = mask;
		int curr_size;
		bool is_cl = true;

		for(int i = 0; i < n; i++) {
			if(t_mask % 2 == 1) idx.push_back(i);
			t_mask = t_mask / 2;
		}
		curr_size = idx.size();

		for (int i = 0; i < curr_size; ++i) {
			for (int j = 0; j < curr_size; ++j)	{
				if(idx[i] == idx[j]) {
					continue;
				}
				if(m[idx[i]][idx[j]] == 0) {
					is_cl = false;
					break;
				}
			}
			if(!is_cl) break;
		}

		if(is_cl) {
			cout << "Found " << k << "-clique on vertices: " << endl;
			for(auto i : idx) {
				cout << i << " ";
			}
			cout << endl;
			return true;
		}
	}
	cout << k << "-clique was not found!" << endl;
	return false;
}

void find_max_clique(ifstream &fin) {
	int n;
	int val;
	fin >> n;
	vector<vector<int>> m(n);

	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			fin >> val;
			m[i].push_back(val);
		}
	}

	int m_cl = 0;
	int m_mask = 0;
	for(int mask = 0; mask < (int) pow(2, n); mask++) {
		vector<int> idx;
		bool is_cl = true;
		int curr_size;
		int t_mask = mask;

		for(int i = 0; i < n; i++) {
			if(t_mask % 2 == 1) idx.push_back(i);
			t_mask = t_mask / 2;
		}
		curr_size = idx.size();

		for (int i = 0; i < curr_size; ++i) {
			for (int j = 0; j < curr_size; ++j)	{
				if(idx[i] == idx[j]) {
					continue;
				}
				if(m[idx[i]][idx[j]] == 0) {
					is_cl = false;
					break;
				}
			}
			if(!is_cl) break;
		}
		if(!is_cl) continue;

		if(curr_size > m_cl) {
			m_cl = curr_size;
			m_mask = mask;
		}
	}

	cout << "Max cl: " << m_cl << endl;
	cout << "Vertices: " << endl;
	for(int i = 0; i < n; i++) {
		if(m_mask % 2 == 1) cout << i << " ";

		m_mask /= 2;
	}
	cout << endl;
}

int32_t main()
{
	int seed = time(0);
	seed = 1751589752;
	cout << seed << endl;

	srand(seed);

	ifstream fin("key_graph.txt");
	ofstream fout("adj_matrices.txt");	

	ifstream fadj("adj_matrices.txt");
	ifstream fcli("adj_matrices.txt");

	int t;
	fin >> t;
	for (int i = 0; i < t; ++i) {
		generate_graph(fin, fout);
		find_max_clique(fadj);
		find_k_clique(fcli, 13);
	}
	return 0;
}
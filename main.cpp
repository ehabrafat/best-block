#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
using namespace std;

bool all(const vector<int>& nums) {
	for (int n : nums) 
		if (n == 0) return false;
	return true;
}

int bestBlock(const vector<unordered_map<string, bool>>& blocks, const vector<string>& req) {
	unordered_map<string, int> placeIdx{};
	for (int i = 0; i < req.size(); ++i) {
		placeIdx[req[i]] = i;
	}
	int n = blocks.size();
	int m = req.size();
	// down prefix
	vector<vector<int>> downPrefix(n, vector<int>(m));
	for (const auto& [place, status] : blocks[0]) {
		if (placeIdx.count(place)) {
			downPrefix[0][placeIdx[place]] = status;
		}
	}
	for (int i = 1; i < n; ++i) {
		for (const auto& [place, status] : blocks[i]) {
			if (placeIdx.count(place)) {
				int idx = placeIdx[place];
				downPrefix[i][idx] = downPrefix[i - 1][idx] + status;
			}
		}
	}
	// up prefix
	vector<vector<int>> upPrefix(n, vector<int>(m));
	for (const auto& [place, status] : blocks[n - 1]) {
		if (placeIdx.count(place)) {
			upPrefix[n - 1][placeIdx[place]] = status;
		}
	}
	for (int i = n - 2; i >= 0; --i) {
		for (const auto& [place, status] : blocks[i]) {
			if (placeIdx.count(place)) {
				int idx = placeIdx[place];
				upPrefix[i][idx] = upPrefix[i + 1][idx] + status;
			}
		}
	}
	
	int minSoFar = INT_MAX;
	int block = -1;
	for (int i = 0; i < n; ++i) {
		int left = 0, right = n - 1;
		int minDis = 0;
		while (left <= right)
		{
			int mid = left + (right - left) / 2;
			int start = max(0, i - mid);
			int end = min(n - 1, i + mid);
			vector<int> cur(m);
			if (start - 1 >= 0) {
				for (int j = 0; j < m; ++j) {
					cur[j] += downPrefix[i][j] - downPrefix[start - 1][j];
				}
			}
			else
			{
				for(int j = 0; j < m; ++j) 
					cur[j] += downPrefix[i][j];
			}
			if (end + 1 < n) {
				for (int j = 0; j < m; ++j) {
					cur[j] += upPrefix[i + 1][j] - upPrefix[end + 1][j];
				}
			}
			else {
				if(i + 1 < n)
					for (int j = 0; j < m; ++j) 
						cur[j] += upPrefix[i + 1][j];
			}
			if (all(cur)) {
				minDis = mid;
				right = mid - 1;
			}
			else
				left = mid + 1;
		}
		if (minDis < minSoFar) {
			minSoFar = minDis;
			block = i;
		}
	}
	return block;
}

int main() {
	vector<unordered_map<string, bool>> blocks{
		{
			{"gym",false},
			{"school",true},
			{"store",false},
		},
		{
			{"gym",true},
			{"school",false},
			{"store",false},
		},
		{
			{"gym",false},
			{"school",true},
			{"store",false},
		},
		{
			{"gym",false},
			{"school",false},
			{"store",true},
		},
	};
	vector<string> req{ "gym", "store" };
	cout << bestBlock(blocks, req); // Block (2)
}
 

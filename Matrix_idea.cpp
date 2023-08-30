#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <cmath>
using namespace std;

class Matrix {
private:
	class Node {
	public:
		bool visited;
		bool is_active;
		Node() : visited(false), is_active(true) {}
	};

	std::vector<std::vector<Node>> data;
	std::vector<int> dirX = { -1, 0, 1, -1, 1, -1, 0, 1 };
	std::vector<int> dirY = { -1, -1, -1, 0, 0, 1, 1, 1 };

public:
	Matrix(int rows, int cols) {
		data.resize(rows, std::vector<Node>(cols));
	}

	std::vector<std::vector<Node>>& getData() {
		return data;
	}

	void ThanosSnap(float percentage) {
		// Calculate number of victims
		float victims = floor(data.size()*data.size() * percentage / 100);
		
		// Collect all node indices
		std::vector<std::pair<int, int>> indices;
		for (int i = 0; i < data.size(); ++i) {
			for (int j = 0; j < data[i].size(); ++j) {
				indices.emplace_back(i, j);
			}
		}

		// Shuffle the indices randomly
		std::shuffle(indices.begin(), indices.end(), std::default_random_engine(std::time(nullptr)));

		// Eliminate victims
		for (int i = 0; i < victims; ++i) {
			int row = indices[i].first;
			int col = indices[i].second;
			data[row][col].is_active = false;
		}
	}

	void showTime() {
		for (std::vector<Node>& row : getData()) {
			for (Node& node : row) {
				std::cout << node.is_active;
			}
			std::cout << "\n";
		}
	}
	
	void findPath(int x1, int y1, int x2, int y2) {

	}
};


int main() {

	Matrix SampleMatrix(3,3);
  //Set 50% Nodes to is_active = false
	SampleMatrix.ThanosSnap(50);
	SampleMatrix.showTime();
	
	return 0;
}

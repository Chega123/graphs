#include "Matrix.h"

using namespace std;

Matrix::Node::Node() : visited(false), is_active(true) {}

Matrix::Matrix(int rows, int cols) {
	data.resize(rows, std::vector<Node>(cols));
	dirX = { -1, 0, 1, -1, 1, -1, 0, 1 };
	dirY = { -1, -1, -1, 0, 0, 1, 1, 1 };
}

void Matrix::ThanosSnap(float percentage) {
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

int Matrix::rows() const {
	return data.size();
}

int Matrix::cols() const {
	if (!data.empty()) return data[0].size();
	return 0;
}

bool Matrix::isActive(int row, int col) const {
	return data[row][col].is_active;
}

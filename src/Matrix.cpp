#include "Matrix.h"

using namespace std;

Matrix::Node::Node() : is_active(true) {}

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

bool Matrix::isInside(int x, int y) {
	return x >= 0 && y >= 0 && x < data[0].size() && y < data.size();
}


bool Matrix::DFS(std::pair<int, int> start, std::pair<int, int> end) {
	std::stack<std::pair<int, int>> s;
	s.push(start);

	while (!s.empty())
	{
		auto [cx, cy] = s.top();
		s.pop();

		if (!isInside(cx, cy) || data[cy][cx].visited[0])
		{
			continue;
		}
		std::cout << "nodo: " << cx << " " << cy << std::endl;
		if (std::make_pair(cx, cy) == end)
		{
			return true;
		}
		data[cy][cx].visited[0] = 1; // ya se visito
		for (int i = 0; i < dirX.size(); i++)
		{
			int newX = cx + dirX[i];
			int newY = cy + dirY[i];
			if (isInside(newX, newY) && !data[newY][newX].visited[0])
			{
				s.push({newX, newY});
			}
			
		}
	}
	
	return false;
}

bool Matrix::BFS(std::pair<int, int> start, std::pair<int, int> end) {
	std::queue<std::pair<int, int>> q;
	q.push(start);

	while (!q.empty())
	{
		auto [cx, cy] = q.front();
		q.pop();

		if (!isInside(cx, cy) || data[cy][cx].visited[1])
		{
			continue;
		}
		std::cout << "nodo: " << cx << " " << cy << std::endl;
		if (std::make_pair(cx, cy) == end)
		{
			return true;
		}
		data[cy][cx].visited[1] = 1; // ya se visito

		for (int i = 0; i < dirX.size(); i++)
		{
			int newX = cx + dirX[i];
			int newY = cy + dirY[i];
			if (isInside(newX, newY) && !data[newY][newX].visited[1])
			{
				q.push({newX, newY});
			}
			
		}
	}
	
}

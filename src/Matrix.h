#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <stack>
#include <queue>

class Matrix {
private:
	class Node {
	public:
		bool is_active;
		int visited[2] = {0, 0}; // 0: DFS  1: BFS
		Node();
	};

	std::vector<std::vector<Node>> data;

public:
	std::vector<int> dirX;
	std::vector<int> dirY;

	Matrix(int rows, int cols);

	void ThanosSnap(float percentage);

    int rows() const;

    int cols() const;

    bool isActive(int row, int col) const;

	bool isInside(int x, int y);

    bool DFS(std::pair<int, int> start, std::pair<int, int> end, std::vector<std::pair<int, int>>& path);
    bool BFS(std::pair<int, int> start, std::pair<int, int> end, std::vector<std::pair<int, int>>& path);
    std::vector<std::pair<int, int>> hillClimbing(int startX, int startY, int goalX, int goalY, int maxIterations);
};

#endif // MATRIX_H

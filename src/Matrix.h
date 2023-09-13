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
		std::pair<int,int>parent;
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
	int calculateHeuristic(int x1,int y1, int x2,int y2);
	int calculatedistance(const std::pair<int,int>& nodeA, const std::pair<int,int>& nodeB );
	std::vector<std::pair<int,int>> recontructPath(std::pair<int,int>&goalNode);
	std::vector<std::pair<int,int>> Astar(int startX,int starY,int goalX,int goalY,int iterationLimit);
};

#endif // MATRIX_H

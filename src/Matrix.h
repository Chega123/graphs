#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <cmath>

class Matrix {
private:
	class Node {
	public:
		bool visited;
		bool is_active;
		Node();
	};

	std::vector<std::vector<Node>> data;
	std::vector<int> dirX;
	std::vector<int> dirY;

public:
	Matrix(int rows, int cols);

	void ThanosSnap(float percentage);

    int rows() const;

    int cols() const;

    bool isActive(int row, int col) const;
};

#endif // MATRIX_H

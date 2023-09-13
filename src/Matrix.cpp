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
	for (size_t i = 0; i < data.size(); ++i) {
		for (size_t j = 0; j < data[i].size(); ++j) {
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
    return x >= 0 && y >= 0 && x < static_cast<int>(data[0].size()) && y < static_cast<int>(data.size());
}

bool Matrix::DFS(std::pair<int, int> start, std::pair<int, int> end, std::vector<std::pair<int, int>>& path) {
    std::stack<std::pair<int, int>> s;
    s.push(start);

    if (!data[start.second][start.first].is_active || !data[end.second][end.first].is_active) {
        std::cout << "Nodos de entrada no validos" << std::endl;
        return false;
    }

    while (!s.empty()) {
        auto [cx, cy] = s.top();
        s.pop();

        if (!isInside(cx, cy) || data[cy][cx].visited[0]) {
            continue;
        }

        std::cout << "Node: " << cx << " " << cy << std::endl;
        path.push_back({cy, cx});

        if (std::make_pair(cx, cy) == end) {
            return true;
        }

        data[cy][cx].visited[0] = 1;

        for (size_t i = 0; i < dirX.size(); i++) {
            int newX = cx + dirX[i];
            int newY = cy + dirY[i];
            if (isInside(newX, newY) && !data[newY][newX].visited[0] && data[newY][newX].is_active) {
                s.push({newX, newY});
            }
        }
    }

    return false;
}

bool Matrix::BFS(std::pair<int, int> start, std::pair<int, int> end, std::vector<std::pair<int, int>>& path) {
    std::queue<std::pair<int, int>> q;
    q.push(start);

    if (!data[start.second][start.first].is_active || !data[end.second][end.first].is_active) {
        std::cout << "Nodos de entrada no validos" << std::endl;
        return false;
    }

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        if (!isInside(cx, cy) || data[cy][cx].visited[1]) {
            continue;
        }

        path.push_back({cy, cx});

        if (std::make_pair(cx, cy) == end) {
            return true;
        }

        data[cy][cx].visited[1] = 1;

        for (size_t i = 0; i < dirX.size(); i++) {
            int newX = cx + dirX[i];
            int newY = cy + dirY[i];
            if (isInside(newX, newY) && !data[newY][newX].visited[1] && data[newY][newX].is_active) {
                q.push({newX, newY});
            }
        }
    }

    return false;
}

 std::vector<std::pair<int, int>> Matrix::hillClimbing(int startX, int startY, int goalX, int goalY, int maxIterations) {
	std::vector<std::pair<int, int>> path;
	std::pair<int, int> currentPos(startX, startY);
	std::pair<int, int> prevPos(-1, -1);
	int iteration = 0;

	while (!(currentPos.first == goalX && currentPos.second == goalY)) {
		path.push_back(currentPos);

		std::vector<std::pair<int, int>> neighbors;

		for (int i = 0; i < 8; ++i) {
			int newX = currentPos.first + dirX[i];
			int newY = currentPos.second + dirY[i];

			if (newX >= 0 && static_cast<size_t>(newX)< data.size() && newY >= 0 && static_cast<size_t>(newY) < data[0].size() &&
				data[newX][newY].is_active && !(newX == prevPos.first && newY == prevPos.second)) {
				neighbors.push_back(std::make_pair(newX, newY));
			}
		}

		if (neighbors.empty()) {
			// No valid neighbors left, indicating no path exists
			std::cout << "No valid path exists." << std::endl;
			return path;
		}

		// Ordenar por distancia euclidiana del objetivo
			// sort(inicio del vector, fin del vector, fumada)
			// Explicacion de fumada
		std::sort(neighbors.begin(), neighbors.end(),
			[goalX, goalY](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				// da y db es la distancia entre el punto actual y el punto final
				// da y db se calculan con pitágoras pero no es necesario sacarle raiz, igual se ordenan
				int da = (a.first - goalX) * (a.first - goalX) +
					(a.second - goalY) * (a.second - goalY);
				int db = (b.first - goalX) * (b.first - goalX) +
					(b.second - goalY) * (b.second - goalY);
				// se ordenan según la distancia de menor a mayor
				return da < db;
			});

		prevPos = currentPos; // Update prevPos before moving
		currentPos = neighbors[0]; // Move to the best neighbor

		iteration++;
		if (iteration >= maxIterations) {
			std::cout << "Terminated due to reaching the maximum number of iterations." << std::endl;
			break;
		}
	}

	return path;
} 

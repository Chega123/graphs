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

int Matrix::calculatedistance(const std::pair<int,int>& nodeA, const std::pair<int,int>& nodeB ){
    int dx= nodeA.first-nodeB.first;
    int dy= nodeA.second-nodeB.second;
    return dx*dx+dy*dy;
}

int Matrix::calculateHeuristic(int x1,int y1, int x2,int y2){
    int dx=x1-x2;
    int dy=y1-y2;
    return dx*dx+dy*dy;
};


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
			[this,goalX, goalY](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				// da y db es la distancia entre el punto actual y el punto final
				// da y db se calculan con pitágoras pero no es necesario sacarle raiz, igual se ordenan
                int da=calculatedistance(a,std::make_pair(goalX, goalY));
                int db=calculatedistance(b,std::make_pair(goalX, goalY));
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
std::vector<std::pair<int,int>> Matrix::recontructPath(std::pair<int,int>&goalNode){
    std::vector<std::pair<int,int>> path;
    std::pair<int,int> currentnode=goalNode;
    while(currentnode!=std::pair<int,int>(-1,-1))
    {
        path.push_back(currentnode);
        currentnode= data[currentnode.second][currentnode.first].parent;
    }
    std::reverse(path.begin(),path.end());
    return path;
}

std::vector<std::pair<int,int>> Matrix::Astar(int startX,int startY,int goalX,int goalY,int iterationLimit){
    std::vector<std::pair<int,int>>path;
    int iterations = 0;
    //nodo q usaremos pa la funcion :D
    struct NodeAstar{
        int x,y,g,h,f;//g=costo desde el inicio, h=distancia al objetivo, f=costo total
        bool operator<(const NodeAstar& other) const {
            return f > other.f; // sobrecarga para coomparar prioridad (mayor prioridad mientras mas bajo es f)
        }
    };

    //verificar  si las coordenadas de inicio estan dentro de la matriz y son nodos activos
    if(!isInside(startX,startY)||!isInside(goalX,goalY)||!data[startY][startX].is_active||!data[goalY][goalX].is_active){
        std::cout<<"No valid start nodes:c"<<std::endl;
        return path;
    }
    
    // Crear un conjunto de nodos cerrados para evitar duplicados
    std::vector<std::vector<bool>> closedSet(data.size(), std::vector<bool>(data[0].size(), false));

    // Cola de prioridad para almacenar los nodos abiertos (osea las opciones a donde se puede mover)
    std::priority_queue<NodeAstar> openSet;

    //nodo de inicio
    NodeAstar starNode{startX,startY,calculateHeuristic(startX, startY, goalX, goalY), 0};
    openSet.push(starNode);

    //registro de los padres de nodos
    std::vector<std::vector<std::pair<int, int>>> cameFrom(data.size(), std::vector<std::pair<int, int>>(data[0].size(), {-1, -1}));

    while(!openSet.empty()&&(iterationLimit == -1 || iterations < iterationLimit)) { // -1 significa sin límite

        //obtener nodo con menor costo total estimado de la cola
        NodeAstar current=openSet.top();
        openSet.pop();
        int x=current.x;
        int y=current.y;


        //Comprobar si llego al objetivo
        if(x==goalX && goalY){
            while(x!=startX||y!=startY){ //reconstruir el camino desde el objetivo hasta el inicio
                path.push_back({x,y});
                auto prev=cameFrom[y][x];
                x=prev.first;
                y=prev.second;
            }
        }

        //marcar nodo actual como visitado
        closedSet[y][x]=true;

        //crear sucessores
        for(size_t i=0;i<dirX.size();i++){

            int newX=x+dirX[i];
            int newY=y+dirY[i];

            //ver si estan dentro o si son activos

            if(isInside(newX,newY)&&data[newY][newX].is_active){
                int Gscore= current.g+1;

                //si tiene costo menor y esta en el closedSet no entra ya que es un nodo visitado
                if(closedSet[newY][newX]&& Gscore>=current.g){continue;}

                if(!closedSet[newY][newX]||Gscore<current.g){
                    //calcular heuristica para el sucesor
                    int h=calculateHeuristic(newX,newY,goalX,goalY);

                    //agregar sucesor al conjunto abierto y actualizar sus valores
                    NodeAstar neighbor{newX,newY,Gscore,h,Gscore+h};
                    openSet.push(neighbor),

                    //registrar al padre del sucesor
                    cameFrom[newY][newX]={x,y};
                }
            }
        }   
        iterations+=1;
    }
    std::cout << "Didn't found path" << std::endl;
    return path;
}

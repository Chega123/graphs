#include "Matrix.h"
#include "OpenGLRenderer.h"

int main() {
    std::cout<<"holis";
    Matrix matrix(100, 100);
    matrix.ThanosSnap(5);

    OpenGLRenderer renderer(matrix);
    renderer.initialize();
    renderer.showDFS(true); // Habilita la visualización del camino DFS
    renderer.showBFS(true); // Habilita la visualización del camino BFS

    std::vector<std::pair<int, int>> dfsPath; // Almacena el camino DFS
    std::vector<std::pair<int, int>> bfsPath; // Almacena el camino BFS

    bool foundDFS = matrix.DFS({15, 58}, {20, 25}, dfsPath);
    bool foundBFS = matrix.BFS({58, 15}, {20, 25}, bfsPath);
    
    std::vector<std::pair<int, int>> hillClimbingPath = matrix.hillClimbing(15, 58, 48, 71, 4000);

    if (!hillClimbingPath.empty()) {
        std::cout << "Camino encontrado por hillClimbing:" << std::endl;
        for (const auto& point : hillClimbingPath) {
            std::cout << "(" << point.first << ", " << point.second << ") ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No se encontró un camino válido por hillClimbing." << std::endl;
    }

    std::cout << "DFS: Nodo final " << (foundDFS ? "encontrado": "no encontrado") << std::endl;
    std::cout << "BFS: Nodo final " << (foundBFS ? "encontrado": "no encontrado") << std::endl;

    // Asigna los caminos DFS y BFS a OpenGLRenderer
    renderer.setDFSPath(dfsPath);
    renderer.setBFSPath(bfsPath);

    renderer.run();

    return 0;
}

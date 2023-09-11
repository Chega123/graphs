#include "Matrix.h"
#include "OpenGLRenderer.h"

int main() {
    Matrix matrix(30, 30);
    matrix.ThanosSnap(30);

    OpenGLRenderer renderer(matrix);
    renderer.initialize();
    renderer.showDFS(true); // Habilita la visualización del camino DFS
    renderer.showBFS(true); // Habilita la visualización del camino BFS

    std::vector<std::pair<int, int>> dfsPath; // Almacena el camino DFS
    std::vector<std::pair<int, int>> bfsPath; // Almacena el camino BFS

    bool foundDFS = matrix.DFS({2, 2}, {20, 25}, dfsPath);
    bool foundBFS = matrix.BFS({5, 4}, {15, 28}, bfsPath);

    std::cout << "DFS: Nodo final " << (foundDFS ? "encontrado": "no encontrado") << std::endl;
    std::cout << "BFS: Nodo final " << (foundBFS ? "encontrado": "no encontrado") << std::endl;

    // Asigna los caminos DFS y BFS a OpenGLRenderer
    renderer.setDFSPath(dfsPath);
    renderer.setBFSPath(bfsPath);

    renderer.run();

    return 0;
}

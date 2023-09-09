#include "Matrix.h"
#include "OpenGLRenderer.h"

int main() {
    Matrix matrix(30, 30);
    matrix.ThanosSnap(30);

    bool foundDFS = matrix.DFS({2, 2}, {20, 25});

    std::cout << "DFS: Nodo final " << (foundDFS ? "encontrado": "no encontrado") << std::endl;

    bool foundBFS = matrix.BFS({5, 4}, {15, 28});

    std::cout << "BFS: Nodo final " << (foundBFS ? "encontrado": "no encontrado") << std::endl;

    OpenGLRenderer renderer(matrix);
    renderer.initialize();
    renderer.run();

    return 0;
}

#include "Matrix.h"
#include "OpenGLRenderer.h"

void Menu(){
    std::cout << "\n-----MENUCITO----\n";
    std::cout << "1. DFS\n";
    std::cout << "2. BFS\n";
    std::cout << "3. Hill Climbing\n";
    std::cout << "4. A*\n";
    std::cout << "5. Exit\n";
}

int main() {
    const int x =100;
    Matrix matrix(x, x);
    matrix.ThanosSnap(20);

    std::vector<std::pair<int, int>> dfsPath; // Almacena el camino DFS
    std::vector<std::pair<int, int>> bfsPath; // Almacena el camino BFS

    bool foundDFS = matrix.DFS({1, 2}, {x-1, x-5}, dfsPath);
    bool foundBFS = matrix.BFS({1, 2}, {x-1, x-5}, bfsPath);
    
    std::vector<std::pair<int, int>> hillClimbingPath = matrix.hillClimbing(15, 5,x - 1 ,x - 5, 200);
    std::vector<std::pair<int, int>> astarPath = matrix.Astar(2, 2, x-1, x-5,200);

    OpenGLRenderer renderer(matrix);
    renderer.initialize();

    while (true) {
        Menu();
        int o;
        std::cin >> o;
        switch (o) {
            case 1:
                renderer.showDFS(true); // Habilita la visualización del camino DFS
                renderer.showBFS(false); // Habilita la visualización del camino BFS
                renderer.showHC(false); // Habilita la visualización del camino BFS
                renderer.showAstar(false); // Habilita la visualización del camino BFS
                if (foundDFS) {
                    renderer.setDFSPath(dfsPath);
                }
                else {
                    std::cout << "Nodos de entrada no validos" << std::endl;
                }
                renderer.run();
            case 2:
                renderer.showDFS(false); // Habilita la visualización del camino DFS
                renderer.showBFS(true); // Habilita la visualización del camino BFS
                renderer.showHC(false); // Habilita la visualización del camino BFS
                renderer.showAstar(false); // Habilita la visualización del camino BFS
                if (foundDFS) {
                    renderer.setBFSPath(bfsPath);
                }
                else {
                    std::cout << "Nodos de entrada no validos" << std::endl;
                }
                renderer.run();
            case 3:
                renderer.showDFS(false); // Habilita la visualización del camino DFS
                renderer.showBFS(false); // Habilita la visualización del camino BFS
                renderer.showHC(true); // Habilita la visualización del camino BFS
                renderer.showAstar(false); // Habilita la visualización del camino BFS
                if (!hillClimbingPath.empty()) {
                    renderer.setHCPath(hillClimbingPath);
                } 
                else {
                    std::cout << "Nodos de entrada no validos" << std::endl;
                }
                renderer.run();
            case 4:
                renderer.showDFS(false); // Habilita la visualización del camino DFS
                renderer.showBFS(false); // Habilita la visualización del camino BFS
                renderer.showHC(false); // Habilita la visualización del camino BFS
                renderer.showAstar(true); // Habilita la visualización del camino BFS
                if (!astarPath.empty()) {
                    renderer.setAStarPath(astarPath);
                } 
                else {
                std::cout << "Nodos de entrada no validos" << std::endl;
                }
                renderer.run();
            case 5:
                break;
            default:
                break;
        }
    }


    return 0;
}

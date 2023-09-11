#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GLFW/glfw3.h>
#include "Matrix.h"  // Asegúrate de que la clase Matrix esté disponible

class OpenGLRenderer {
private:
    GLFWwindow* window;
    Matrix& matrix;
    float nodeSize;
    float spacing;  // Espacio entre nodos
    float screenWidth;
    float screenHeight;

    void drawNode(int x, int y);
    void drawConnections(int i, int j);
    void drawLink(int x1, int y1, int x2, int y2);
    std::vector<std::pair<int, int>> dfsPath;
    std::vector<std::pair<int, int>> bfsPath;
    bool showDFSPath;
    bool showBFSPath;

public:
    OpenGLRenderer(Matrix& m, float screenWidth = 800.0f, float screenHeight = 800.0f);
    ~OpenGLRenderer();

    void initialize();
    void run();

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    void showDFS(bool show);
    void showBFS(bool show);
    void setDFSPath(const std::vector<std::pair<int, int>>& path);
    void setBFSPath(const std::vector<std::pair<int, int>>& path);
};

#endif

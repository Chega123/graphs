#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GLFW/glfw3.h>
#include "Matrix.h"  // Asegúrate de que la clase Matrix esté disponible

struct Color
{
	float r, g, b, a;
	static const Color Black;
	static const Color White;
	static const Color Grey;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
    static const Color Purple;
    static const Color Cyan;
    static const Color Orange;
};

class OpenGLRenderer {
private:
    GLFWwindow* window;
    Matrix& matrix;
    float nodeSize;
    float spacing;  // Espacio entre nodos
    float screenWidth;
    float screenHeight;

    void drawNode(int x, int y, Color color);
    void drawConnections(int i, int j, Color color);
    void drawLink(int x1, int y1, int x2, int y2, Color color);
    std::vector<std::pair<int, int>> dfsPath;
    std::vector<std::pair<int, int>> bfsPath;
    std::vector<std::pair<int, int>> hcPath;
    std::vector<std::pair<int, int>> astarPath;
    bool showDFSPath;
    bool showBFSPath;
    bool showHCPath;
    bool showAStarPath;

public:
    OpenGLRenderer(Matrix& m, float screenWidth = 800.0f, float screenHeight = 800.0f);
    ~OpenGLRenderer();

    void initialize();
    void run();

    void showDFS(bool show);
    void showBFS(bool show);
    void showHC(bool show);
    void showAstar(bool show);
    void handleMouseClick(double x, double y);
    void setDFSPath(const std::vector<std::pair<int, int>>& path);
    void setBFSPath(const std::vector<std::pair<int, int>>& path);
    void setHCPath(const std::vector<std::pair<int, int>>& path);
    void setAStarPath(const std::vector<std::pair<int, int>>& path);
};

#endif

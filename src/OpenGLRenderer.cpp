#include "OpenGLRenderer.h"
#include <iostream>
#include <ostream>

const struct Color Color::Black = { 0, 0, 0, 0 };
const struct Color Color::White = { 1, 1, 1, 1 };
const struct Color Color::Grey = { 0.5f, 0.5f, 0.5f, 0.3f };
const struct Color Color::Red = { 1, 0, 0, 1 };
const struct Color Color::Green = { 0, 1, 0, 1 };
const struct Color Color::Blue = { 0, 0, 1, 1 };
const struct Color Color::Purple = { 0.5, 0, 0.5, 1 };

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
        glfwGetCursorPos(window, &x, &y);
        renderer->handleMouseClick(x, y);
        // std::cout << "Click en: " << x << " " << y << std::endl;
    }
}

OpenGLRenderer::OpenGLRenderer(Matrix& m, float w, float h)
    : window(nullptr), matrix(m), screenWidth(w), screenHeight(h) {
    nodeSize = 2.0f / ( 2*matrix.cols() + 1);
    spacing = nodeSize;
    std::cout << "Node Size: " << nodeSize << std::endl;
}

OpenGLRenderer::~OpenGLRenderer() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void OpenGLRenderer::initialize() {
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(screenWidth, screenHeight, "Matrix Graph", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear ventana" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowUserPointer(window, this);
}


void OpenGLRenderer::drawNode(int row, int col, Color color) {
    glColor4f(color.r, color.g, color.b, color.a);  // verde

    const int segments = 20;
    const float angleIncrement = 2.0f * M_PI / segments;

    float xCenter = -1 + (col + 0.5) * (nodeSize + spacing);
    float yCenter = -1 + (row + 0.5) * (nodeSize + spacing);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xCenter, yCenter);
    for (int i = 0; i < segments; i++)
    {
        float angle = i * angleIncrement;
        float xOffset = cos(angle) * nodeSize * 0.5;
        float yOffset = sin(angle) * nodeSize * 0.5;
        glVertex2f(xCenter + xOffset, yCenter + yOffset);
    }
    glEnd();
    
}

void OpenGLRenderer::drawConnections(int i, int j, Color color) {
    for (int dir = 0; dir < 8; dir++) {
        int ni = i + matrix.dirX[dir];
        int nj = j + matrix.dirY[dir];

        if (ni >= 0 && ni < matrix.rows() && nj >= 0 && nj < matrix.cols())
        {
            if (matrix.isActive(ni, nj))
            {
                drawLink(i, j, ni, nj, color);
            }
            
        }
        
    }
}

void OpenGLRenderer::drawLink(int i1, int j1, int i2, int j2, Color color) {
    glColor4f(color.r, color.g, color.b, color.a);

    float x1 = -1 + (j1 + 0.5) * (nodeSize + spacing);
    float y1 = -1 + (i1 + 0.5) * (nodeSize + spacing);

    float x2 = -1 + (j2 + 0.5) * (nodeSize + spacing);
    float y2 = -1 + (i2 + 0.5) * (nodeSize + spacing);

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void OpenGLRenderer::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < matrix.rows(); i++) {
            for (int j = 0; j < matrix.cols(); j++) {
                if (matrix.isActive(i, j)) {
                    drawNode(i, j, Color::Green);
                    drawConnections(i, j, Color::Grey);
                }
            }
        }
        if (showDFSPath) {
            glBegin(GL_LINE_STRIP);
            for (const auto& point : dfsPath) {
                drawNode(point.first, point.second, Color::Red);
            }
            glEnd();
        }

        if (showBFSPath) 
        {
            glBegin(GL_LINE_STRIP);
            for (const auto& point : bfsPath) 
            {
                int x = point.first;
                int y = point.second;
                if (matrix.data[y][x].visited[0] == 1)
                    drawNode(x, y, Color::Purple);
                else
                    drawNode(x, y, Color::Blue);
            }
            glEnd();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

/* void OpenGLRenderer::handleMouseClick(double x, double y) {
    int row = (y / (nodeSize + spacing)) * matrix.rows();
    int col = (x / (nodeSize + spacing)) * matrix.cols();

    std::cout << "Row: " << row << " Col: " << col << std::endl;

    if (row < matrix.rows() && col < matrix.cols()) {
        if (matrix.isActive(row, col)) {
            // matrix.toggleNode(row, col);// cambiar a nodos del usuario
        }
    }
} */

void OpenGLRenderer::handleMouseClick(double x, double y) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    double normalizedX = x / windowWidth;
    double normalizedY = 1.0 - (y / windowHeight);  // Invertir Y

    double glX = normalizedX * 2.0 - 1.0;
    double glY = normalizedY * 2.0 - 1.0;

    int row = (glY + 1) / 2 * matrix.rows();  // Ajustar para el rango [0, matrix.rows()]
    int col = (glX + 1) / 2 * matrix.cols();  // Ajustar para el rango [0, matrix.cols()]

    std::cout << "Row: " << row << " Col: " << col << std::endl;

    if (row < matrix.rows() && col < matrix.cols()) {
        if (matrix.isActive(row, col)) {
            // matrix.toggleNode(row, col);// cambiar a nodos del usuario
        }
    }
}

void OpenGLRenderer::setDFSPath(const std::vector<std::pair<int, int>>& path) {
    dfsPath = path;
}

void OpenGLRenderer::setBFSPath(const std::vector<std::pair<int, int>>& path) {
    bfsPath = path;
}

void OpenGLRenderer::showDFS(bool show) {
    showDFSPath = show;
}

void OpenGLRenderer::showBFS(bool show) {
    showBFSPath = show;
}

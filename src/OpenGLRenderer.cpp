#include "OpenGLRenderer.h"
#include <iostream>

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
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetWindowUserPointer(window, this);
}


void OpenGLRenderer::drawNode(int row, int col) {
    glColor3f(0, 1, 0);  // verde

    const int segments = 200;
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

void OpenGLRenderer::drawConnections(int i, int j) {
    for (int dir = 0; dir < 8; dir++) {
        int ni = i + matrix.dirX[dir];
        int nj = j + matrix.dirY[dir];

        if (ni >= 0 && ni < matrix.rows() && nj >= 0 && nj < matrix.cols())
        {
            if (matrix.isActive(ni, nj))
            {
                drawLink(i, j, ni, nj);
            }
            
        }
        
    }
}

void OpenGLRenderer::drawLink(int i1, int j1, int i2, int j2) {
    glColor3f(0.4f, 0.4f, 0.4f); // color del enlace (gris claro)

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
                    drawConnections(i, j);
                    drawNode(i, j);
                }
            }
        }
        if (showDFSPath) {
            glColor3f(1, 0, 0); // Rojo para el camino DFS
            glBegin(GL_LINE_STRIP);
            for (const auto& point : dfsPath) {
                float x = -1 + (point.second + 0.5) * (nodeSize + spacing);
                float y = -1 + (point.first + 0.5) * (nodeSize + spacing);
                glVertex2f(x, y);
            }
            glEnd();
        }

        if (showBFSPath) {
            glColor3f(0, 0, 1); // Azul para el camino BFS
            glBegin(GL_LINE_STRIP);
            for (const auto& point : bfsPath) {
                float x = -1 + (point.second + 0.5) * (nodeSize + spacing);
                float y = -1 + (point.first + 0.5) * (nodeSize + spacing);
                glVertex2f(x, y);
            }
            glEnd();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void OpenGLRenderer::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Obtener el objeto OpenGLRenderer
        OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
        
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        
        my = height - my;
        float nx = mx / (float)width * renderer->matrix.cols() * renderer->nodeSize;
        float ny = my / (float)height * renderer->matrix.rows() * renderer->nodeSize;
        
        int row = (int)(ny / renderer->nodeSize);
        int col = (int)(nx / renderer->nodeSize);
        
        if (renderer->matrix.isActive(row, col)) {
            std::cout << "Clicked on matrix coordinates (" << row << ", " << col << ")" << std::endl;
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

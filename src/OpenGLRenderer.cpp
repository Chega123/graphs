#include "OpenGLRenderer.h"
#include <iostream>


OpenGLRenderer::OpenGLRenderer(Matrix& m, float w, float h)
    : matrix(m), screenWidth(w), screenHeight(h), window(nullptr) {

    float totalNodesH = matrix.cols();
    float totalNodesV = matrix.rows();

    // Asumiendo un porcentaje fijo para el espacio total que los espacios ocuparán.
    float spacingPercentage = 0.1; // 10% del total, ajusta según tus necesidades
    float totalSpace = 2.0 * spacingPercentage; // el rango completo [-1, 1] multiplicado por el porcentaje

    float totalSpacesH = totalNodesH - 1;
    float totalSpacesV = totalNodesV - 1;

    spacing = 3 * totalSpace / (totalSpacesH + totalSpacesV); // divido entre el total de espacios en ambas direcciones

    nodeSize = (2.0 - totalSpace) / (totalNodesH + totalNodesV); // el espacio restante después de restar el totalSpace
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


void OpenGLRenderer::drawNode(int x, int y) {
    glColor3f(0, 1, 0);  // verde

    float adjustedSize = nodeSize - spacing;
    float totalWidth = nodeSize * matrix.cols() + spacing * (matrix.cols() - 1);
    float totalHeight = nodeSize * matrix.rows() + spacing * (matrix.rows() - 1);

    // Centrando el diseño completo de nodos en el espacio de coordenadas.
    float startX = -1.0 + (2.0 - totalWidth) / 2.0;
    float startY = 0.7 - nodeSize - (2.0 - totalHeight) / 2.0;

    float center_x = startX + x * (nodeSize + spacing);
    float center_y = startY - y * (nodeSize + spacing);

    if (x % 10 == 0 && y % 10 == 0) {
        std::cout << "Node en: " << x << " , " << y << " center_x: " << center_x
                  << " center_y: " << center_y << std::endl;

        glColor3f(0, 1, 1);  // verde
    }

    int segments = 100;

    glBegin(GL_POLYGON);
    for(int i = 0; i < segments; i++) {
        float theta = (float)(i) / segments * 2.0 * 3.1415926;
        float dx = adjustedSize / 2 * cosf(theta);
        float dy = adjustedSize / 2 * sinf(theta);
        glVertex2f(center_x + dx, center_y + dy);
    }
    glEnd();
}
void OpenGLRenderer::drawLink(int x1, int y1, int x2, int y2) {
    glColor3f(0.7f, 0.7f, 0.7f); // color del enlace (gris claro)

    glBegin(GL_LINES);
    glVertex2f(x1 * (nodeSize + spacing), y1 * (nodeSize + spacing));
    glVertex2f(x2 * (nodeSize + spacing), y2 * (nodeSize + spacing));
    glEnd();
}

void OpenGLRenderer::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < matrix.rows(); i++) {
            for (int j = 0; j < matrix.cols(); j++) {
                if (matrix.isActive(i, j)) {
                    drawNode(i, j);
                    // dibujar conexiones creo
                }
            }
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

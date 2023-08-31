#include "OpenGLRenderer.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(Matrix& m, float w, float h)
    : matrix(m), screenWidth(w), screenHeight(h), window(nullptr) {

    float totalNodesH = matrix.cols();
    float totalNodesV = matrix.rows();

    // El 0.9 es para asegurarnos de que ocupamos el 90% del espacio de la pantalla.
    float availableWidth = 0.9f * 2.0f;  // -1 a 1 en coordenadas OpenGL
    float availableHeight = 0.9f * 2.0f;

    // Calculando el tamaño del nodo y el espaciado para caber dentro del área disponible.
    nodeSize = availableWidth / (totalNodesH + totalNodesV);
    spacing = 0.1f * nodeSize;  // El espaciado es el 10% del tamaño del nodo.
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

    // El margen es el 5% del total de la pantalla (anteriormente asumimos que íbamos a ocupar el 90%)
    float marginLeft = 0.05f * 2.0f;  // -1 a 1 en coordenadas OpenGL
    float marginTop = 0.05f * 2.0f;

    float center_x = marginLeft + (x * (nodeSize + spacing));
    float center_y = 1.0f - (marginTop + y * (nodeSize + spacing)); // El "1.0f -" es porque OpenGL tiene y positivo hacia arriba

    int segments = 100;

    glBegin(GL_POLYGON);
    for(int i = 0; i < segments; i++) {
        float theta = (float)(i) / segments * 2.0 * 3.1415926;
        float dx = adjustedSize / 2 * cosf(theta);
        float dy = adjustedSize / 2 * sinf(theta);
        glVertex2f(center_x + dx, center_y - dy);  // Invierte dy para que el nodo se dibuje correctamente
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

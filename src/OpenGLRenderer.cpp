#include "OpenGLRenderer.h"
#include <iostream>


OpenGLRenderer::OpenGLRenderer(Matrix& m, float w, float h) // desaparecer la mayoria de codigo
    : matrix(m), screenWidth(w), screenHeight(h), window(nullptr) {
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


void OpenGLRenderer::drawNode(int row, int col) { // entener lo que pasa
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

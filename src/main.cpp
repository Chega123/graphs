#include "Matrix.h"
#include "OpenGLRenderer.h"

int main() {
    Matrix matrix(100, 100);
    matrix.ThanosSnap(20);

    OpenGLRenderer renderer(matrix);
    renderer.initialize();
    renderer.run();

    return 0;
}

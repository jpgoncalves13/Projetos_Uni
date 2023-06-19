#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "scale.hpp"
#include "../../shared/matrixOp.hpp"

Scale::Scale(float x, float y, float z) : Transformation(x,y,z) {

}

/**
 * Apply the transformation to a matrix
 * Multiplies matrices
 * @param matrix the current matrix
 */
void Scale::applyTransformation(float *matrix){
    glScalef(this->getX(), this->getY(), this->getZ());
    float m2[16] = { this->getX(), 0, 0, 0,
                     0, this->getX(), 0, 0,
                     0, 0, this->getZ(), 0,
                     0, 0, 0, 1};

    float res[16] = {0, 0, 0, 0,
                     0,0, 0, 0,
                     0,0,0,0,
                     0,0, 0,0};
    multiplyMatrixMatrix(matrix, m2, res);
    for(int i = 0; i < 16; i++) {
        matrix[i] = res[i];
    }
}

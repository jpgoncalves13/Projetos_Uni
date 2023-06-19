#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "rotation.hpp"
#include "../../shared/matrixOp.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

/**
 * Sets all the parameters
 * @param x
 * @param y
 * @param z
 * @param angle
 * @param startCounter
 * @param duration360
 */
Rotation::Rotation(float x, float y, float z, float angle, double startCounter, float duration360) : Transformation(x, y, z), angle(angle), startCounter(startCounter), duration360(duration360) {
}

/**
 * Calculates the angle of the rotation
 * Two possibilities:
 * - by time
 * - fixed
 * @return the angle
 */
double Rotation::getRotationAngle(){
    double rAngle;
    if (duration360 != 0) {
        if (startCounter == 0) {
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Time since the beginning of the program
        }

        double currentTime = glutGet(GLUT_ELAPSED_TIME); // Current time
        double rotationTime = (currentTime - startCounter) / 1000; // In seconds

        if (rotationTime > duration360) {
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Set the counter to the current moment
        }

        rAngle = (double)rotationTime * 360 / duration360; // Angle in radians
    }
    else rAngle = this->getAngle();

    return rAngle;
}

/**
 * Apply the transformation to a matrix
 * Multiplies matrices
 * @param matrix the current matrix
 */
void Rotation::applyTransformation(float *matrix) {
    glRotatef(this->getRotationAngle(), this->getX(), this->getY(), this->getZ());
    float m2[16] = { this->getX()*this->getX() + 1 * (1- this->getX()*this->getX()) * (float) cos(this->getRotationAngle()), this->getX()*this->getY() * (1-(float)cos(this->getRotationAngle())) - this->getZ() * (float) sin(this->getRotationAngle()), this->getX()*this->getZ() * (1-(float)cos(this->getRotationAngle())) + this->getY() * (float) sin(this->getRotationAngle()), 0,
                     this->getX()*this->getY() * (1-(float)cos(this->getRotationAngle())) + this->getZ() * (float) sin(this->getRotationAngle()), this->getY()*this->getY() + 1 * (1- this->getY()*this->getY()) * (float) cos(this->getRotationAngle()), this->getY()*this->getZ() * (1-(float)cos(this->getRotationAngle())) - this->getX() * (float) sin(this->getRotationAngle()), 0,
                     this->getX()*this->getZ() * (1-(float)cos(this->getRotationAngle())) - this->getY() * (float) sin(this->getRotationAngle()), this->getY()*this->getZ() * (1-(float)cos(this->getRotationAngle())) + this->getX() * (float) sin(this->getRotationAngle()), this->getZ()*this->getZ() + 1 * (1- this->getZ()*this->getZ()) * (float) cos(this->getRotationAngle()), 0,
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

float Rotation::getAngle() {
    return angle;
}

void Rotation::setAngle(float angle) {
    this->angle = angle;
}

/**
 * Apply the transformation to a point
 * @param base
 * @param radius
 */
void Rotation::applyTransformationToPoint(Point* base, float* radius) {
    if (this->getX() == 0 && this->getY() == 0 && this->getZ() == 1){
        float baseX = base->getX(), baseY = base->getY();
        base->setX(baseX * cos(M_PI/180*getRotationAngle()) + baseY*(-sin(M_PI/180*getRotationAngle())));
        base->setY(baseX * sin(M_PI/180*getRotationAngle()) + baseY * cos(M_PI/180*getRotationAngle()));
    }
    else if (this->getX() == 0 && this->getY() == 1 && this->getZ() == 0){
        float baseX = base->getX(), baseZ = base->getZ();
        base->setX(baseX * cos(M_PI/180*getRotationAngle()) + baseZ * sin(M_PI/180*getRotationAngle()));
        base->setZ(baseX * (-sin(M_PI/180*getRotationAngle())) + baseZ * cos(M_PI/180*getRotationAngle()));
    }
    else if (this->getX() == 1 && this->getY() == 0 && this->getZ() == 0){
        float baseY = base->getY(), baseZ = base->getZ();
        base->setY(baseY * cos(M_PI/180*getRotationAngle()) + baseZ * (-sin(M_PI/180*getRotationAngle())));
        base->setZ(baseY * sin(M_PI/180*getRotationAngle()) + baseZ * cos(M_PI/180*getRotationAngle()));
    }
}

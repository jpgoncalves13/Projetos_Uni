#include "transformation.hpp"

void Transformation::applyTransformation(float *matrix)
{
}



Transformation::Transformation(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

float Transformation::getX() {
    return x;
}

void Transformation::setX(float x) {
    Transformation::x = x;
}

float Transformation::getY() {
    return y;
}

void Transformation::setY(float y) {
    Transformation::y = y;
}

float Transformation::getZ() {
    return z;
}

void Transformation::setZ(float z) {
    Transformation::z = z;
}








#include "plane.hpp"

Plane::Plane(){

}

Plane::Plane(float a, float b, float c, float d) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}

void Plane::setCoefficients(float a, float b, float c, float d) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}


bool Plane::inRightSide(Point p) {
    float dist = (a * p.getX() + b * p.getY() + c * p.getZ() + d);
    return dist > 0;
}

float Plane::distanceToPoint(Point p) {
    return (a * p.getX() + b * p.getY() + c * p.getZ() + d);
}


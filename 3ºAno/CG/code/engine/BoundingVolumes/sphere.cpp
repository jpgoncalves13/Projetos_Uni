#include "sphere.hpp"
#include "plane.hpp"
#include "../../shared/matrixOp.hpp"
#include <cmath>

#define INDEX(row, col) ((col) * 4 + (row) - 5)

Sphere::Sphere(float radius, Point center) : radius(radius), center(center) {

}

Sphere::Sphere() {
}

void Sphere::setRadius(float radius) {
    this->radius = radius;
}

float Sphere::getRadius() {
    return radius;
}

void Sphere::setCenter(Point center) {
    this->center.setPoint(center.getX(), center.getY(), center.getZ());
}

Point Sphere::getCenter() {
    return this->center;
}


bool Sphere::test(Plane *planes, float *actual_matrix) {
    float res[4];
    float v[4] = {this->center.getX(), this->center.getY(), this->center.getZ(), 1.0};
    multiplyMatrixVector(actual_matrix, v ,res);
    Point centro = Point(res[0], res[1], res[2]);

    float scale = sqrt(actual_matrix[0] * actual_matrix[0] + actual_matrix[1] * actual_matrix[1] + actual_matrix[2] * actual_matrix[2]);
    scale += sqrt(actual_matrix[4] * actual_matrix[4] + actual_matrix[5] * actual_matrix[5] + actual_matrix[6] * actual_matrix[6]);
    scale += sqrt(actual_matrix[8] * actual_matrix[8] + actual_matrix[9] * actual_matrix[9] + actual_matrix[10] * actual_matrix[10]);

    scale /= 3.0f;
    float radius = this->radius * scale;

    for (int i=0; i<6; i++) {
        float distance = planes[i].distanceToPoint(centro);
        if (distance < -radius) {
            return false;
        }
    }

    return true;
}

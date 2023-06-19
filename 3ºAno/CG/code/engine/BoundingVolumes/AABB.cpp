#include "AABB.hpp"
#include "plane.hpp"
#include "../../shared/matrixOp.hpp"
#include <cmath>

#define INDEX(row, col) ((col) * 4 + (row) - 5)

AABB::AABB(Point* corners) {
    for(int i = 0; i < 8; i++) {
        this->corners[i].setPoint(corners[i].getX(), corners[i].getY(), corners[i].getZ());
    }
}

AABB::AABB() {
}

void AABB::setCorners(Point* corners) {
    for(int i = 0; i < 8; i++) {
        this->corners[i].setPoint(corners[i].getX(), corners[i].getY(), corners[i].getZ());
    }
}   

Point* AABB::getCorners() {
    return this->corners;
}

bool AABB::test(Plane *planes, float* actual_matrix) {
    bool ret = false;
    Point cs[8];

    for(int i = 0; i < 8; i++) {
        float v[4] = {this->corners[i].getX(), this->corners[i].getY(), this->corners[i].getZ(), 1.0f};
        float res[4];
        multiplyMatrixVector(actual_matrix, v, res);
        cs[i].setPoint(res[0], res[1], res[2]);
    }

    for(int i=0; i<6; i++) {
        for(int j=0; j<8 && !ret; j++) {
            ret = planes[i].inRightSide(cs[j]);
        }

        if (!ret) return false;
        else ret = false;
    }

    return true;
}
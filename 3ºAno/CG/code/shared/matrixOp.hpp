#include "point.hpp"

#ifndef MATRIX_OPERATIONS
#define MATRIX_OPERATIONS


void cross(const float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void buildRotMatrix(float *x, float *y, float *z, float *m);
void multiplyMatrixVector(float *m, float *v, float *res);
void multiplyMatrixMatrix(float *m1, float *m2, float *res);
void multiplyMatrixPointMatrix(float *m1, Point *m2, Point *res);
void multiplyPointMatrixMatrix(Point *m1, float *m2, Point *res);
void multiplyVectorPointMatrix(float *v, Point *m, Point *res);
void multiplyPointVectorVector(Point *v1, float *v2, Point *res);

#endif


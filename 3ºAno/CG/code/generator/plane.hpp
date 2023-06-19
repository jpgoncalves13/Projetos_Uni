#include <vector>
#include "../shared/point.hpp"

#ifndef PLANE_H
#define PLANE_H

vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index, vector<float>* normals, vector<float>* textCoord);

#endif
#include <vector>
#include "../shared/point.hpp"

#ifndef SPHERE_H
#define SPHERE_H

vector<float> generateSphere(float radius, int slices, int stacks, Point start, int *index, vector<unsigned int> *indexes, vector<float> *normals, vector<float> *textCoords);

#endif
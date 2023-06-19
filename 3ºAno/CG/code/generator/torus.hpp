#include <vector>

#ifndef TORUS_H
#define TORUS_H

std::vector<float> generateTorus(float innerRadius, float outerRadius, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoords);

#endif
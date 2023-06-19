#include <vector>

#ifndef CONE_H
#define CONE_H

std::vector<float> generateCone(float radius, float height, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoords);

#endif
#include <vector>

#ifndef RING_H
#define RING_H

std::vector<float> generateRing (float outerRadius, float innerRadius, int n, float minScale, float maxScale, float minAngle, float maxAngle, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoord, char** args);
#endif
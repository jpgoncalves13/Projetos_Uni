#include <vector>

#ifndef CYLINDER_H
#define CYLINDER_H

std::vector<float> generateCylinder(float radius, float height, int slices,std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* texCoords);

#endif
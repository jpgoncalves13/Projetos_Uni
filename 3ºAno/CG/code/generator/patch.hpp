#include <vector>
#include <string>
#include "../shared/point.hpp"

#ifndef PATCH_H
#define PATCH_H

vector<float> generatePatches(vector<Point> controlPoints, vector<unsigned int> patchesIndexes, int tesselation, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoord, float* radiusSphere, Point approxCenter);
vector<Point> readPatch(const std::string& fileName, std::vector<unsigned int>* indexes, Point* approxCenter);

#endif
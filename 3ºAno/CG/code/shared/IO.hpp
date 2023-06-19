#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include "triangle.hpp"
#include "../engine/BoundingVolumes/volume.hpp"

void writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord, float radius, Point center);
void writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord, Point *corners);
vector<float> reader(const string& fileName, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoords, Volume **volume);
#endif

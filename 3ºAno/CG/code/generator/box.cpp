#include "plane.hpp"
#include "box.hpp"

/**
 * We generate a box generating each of its 6 planes
 * There is a function to define a plane given the initial point and the direction of generation
 * @param side
 * @param grid
 * @param start
 * @param indexes
 * @param index
 * @param normals
 * @param textCoord
 * @return
 */
vector<float> generateBox(float side, int grid, Point start, vector<unsigned int> *indexes, int *index, vector<float>* normals, vector<float>* textCoord) {
    vector<float> vertices = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() - side / 2, start.getZ()-side / 2), true, indexes, index, normals,  textCoord);
    vector<float> aux = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() - side / 2), false, indexes, index, normals, textCoord);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() + side / 2), false, indexes, index, normals, textCoord);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() + side / 2), true, indexes, index, normals, textCoord);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(1, -1, 0), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() + side / 2), false, indexes, index, normals, textCoord);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(1, -1, 0), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() - side / 2), true, indexes, index, normals, textCoord);
    vertices.insert(vertices.end(), aux.begin(), aux.end());

    return vertices;
}

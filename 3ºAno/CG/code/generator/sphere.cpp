#define _USE_MATH_DEFINES
#include <math.h>
#include "sphere.hpp"

/**
 * Generates a group of triangles that combined approximate a sphere with a given radius using slices and stacks
 * The sphere is centered in the origin (0,0,0)
 * @param radius the radius of the sphere
 * @param slices vertical divisions of the sphere
 * @param stacks horizontal divisions of the sphere
 * @return
 */
vector<float> generateSphere(float radius, int slices, int stacks, Point start, int *index, vector<unsigned int> *indexes, vector<float> *normals, vector<float> *textCoords) {
    vector<float> vertices;

    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = M_PI / stacks;
    float deltaX = 1.0f / slices;
    float deltaY = 1.0f / stacks;

    // Spherical coordinates for vertices and for normals
    // Textures:
    // X is mapped to slices
    // Y is mapped to stacks

    for (int i = 0; i < slices; i++) {
        vertices.push_back(start.getX());
        vertices.push_back(start.getY()-radius);
        vertices.push_back(start.getZ());
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        textCoords->push_back((i+0.5) * deltaX);
        textCoords->push_back(0);

        vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) sin((float) i * sliceStep));
        vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) cos((float) i * sliceStep));
        normals->push_back((float) cos(-M_PI / 2 + stackStep) * (float) sin((float) i * sliceStep));
        normals->push_back((float) sin(-M_PI / 2 + stackStep));
        normals->push_back((float) cos(-M_PI / 2 + stackStep) * (float) cos((float) i * sliceStep));
        textCoords->push_back(i * deltaX);
        textCoords->push_back(deltaY);

        vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) sin((float) (i + 1) * sliceStep));
        vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) cos((float) (i + 1) * sliceStep));
        normals->push_back((float) cos(-M_PI / 2 + stackStep) * (float) sin((float) (i+1) * sliceStep));
        normals->push_back((float) sin(-M_PI / 2 + stackStep));
        normals->push_back((float) cos(-M_PI / 2 + stackStep) * (float) cos((float) (i+1) * sliceStep));
        textCoords->push_back((i+1) * deltaX);
        textCoords->push_back(deltaY);

        indexes->push_back(*index);
        indexes->push_back(*index + 2);
        indexes->push_back(*index + 1);

        (*index)++;

        for (int j = 1; j < stacks - 1; j++) {
            vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * sin((float) i * sliceStep));
            vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * cos((float) i * sliceStep));
            normals->push_back((float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * (float) sin((float) i * sliceStep));
            normals->push_back((float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            normals->push_back((float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * (float) cos((float) i * sliceStep));
            textCoords->push_back(i * deltaX);
            textCoords->push_back((j+1) * deltaY);

            vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * sin((float) (i + 1) * sliceStep));
            vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * cos((float) (i + 1) * sliceStep));
            normals->push_back((float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * (float) sin((float) (i + 1) * sliceStep));
            normals->push_back((float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            normals->push_back((float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * (float) cos((float) (i + 1) * sliceStep));
            textCoords->push_back((i+1) * deltaX);
            textCoords->push_back((j+1) * deltaY);

            indexes->push_back(*index);
            indexes->push_back(*index + 1);
            indexes->push_back(*index + 2);

            indexes->push_back(*index + 1);
            indexes->push_back(*index + 3);
            indexes->push_back(*index + 2);

            (*index) += 2;
        }

        vertices.push_back(start.getX());
        vertices.push_back(start.getY() + radius);
        vertices.push_back(start.getZ());
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        textCoords->push_back((i+0.5) * deltaX);
        textCoords->push_back(1);

        indexes->push_back(*index);
        indexes->push_back(*index + 1);
        indexes->push_back(*index + 2);

        (*index) += 3;
    }

    return vertices;
}
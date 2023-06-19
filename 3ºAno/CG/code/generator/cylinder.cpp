#define _USE_MATH_DEFINES
#include <math.h>
#include "cylinder.hpp"

/**
 * Generates a group of triangles, making a cylinder of given radius and height
 * @param radius the radius of the base and top circles of the cylinder
 * @param height the height of the cylinder
 * @param slices vertical divisions of the sphere
 * @return a list of generated triangles
 */
std::vector<float> generateCylinder(float radius, float height, int slices,std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* texCoords) {
    std::vector<float> vertices;
    float aux = height / 2;
    float delta = 1.0f / slices;
    float sliceStep = (2 * M_PI) / slices;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        /* Base */
        vertices.push_back(0);
        vertices.push_back(-aux);
        vertices.push_back(0);
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        texCoords->push_back(0.8125f); // Special texture format -> See report
        texCoords->push_back(0.1875f);

        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        texCoords->push_back(0.8125f + 0.1875f * sin( i * sliceStep));
        texCoords->push_back(0.1875f + 0.1875f * cos(i * sliceStep));

        vertices.push_back(radius * (float)sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        texCoords->push_back(0.8125f + 0.1875f * sin( (i+1) * sliceStep));
        texCoords->push_back(0.1875f + 0.1875f * cos((i+1) * sliceStep));

        /* Lateral */
        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));
        normals->push_back((float) sin(i * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos(i * sliceStep));
        texCoords->push_back(i * delta);
        texCoords->push_back(0.375f);

        vertices.push_back(radius * (float)sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));
        normals->push_back((float) sin((i+1) * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos((i+1) * sliceStep));
        texCoords->push_back((i+1) * delta);
        texCoords->push_back(0.375f);

        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));
        normals->push_back((float) sin(i * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos(i * sliceStep));
        texCoords->push_back(i * delta);
        texCoords->push_back(1);

        vertices.push_back(radius * (float)sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));
        normals->push_back((float) sin((i+1) * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos((i+1) * sliceStep));
        texCoords->push_back((i+1) * delta);
        texCoords->push_back(1);

        /* Top */
        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        texCoords->push_back(0.4375f + 0.1875f * sin( i * sliceStep));
        texCoords->push_back(0.1875f + 0.1875f * cos(i * sliceStep));

        vertices.push_back(radius * (float) sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        texCoords->push_back(0.4375f + 0.1875f * sin((i+1) * sliceStep));
        texCoords->push_back(0.1875f + 0.1875f * cos((i+1) * sliceStep));

        vertices.push_back(0);
        vertices.push_back(aux);
        vertices.push_back(0);
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        texCoords->push_back(0.4375f);
        texCoords->push_back(0.1875f);

        /* Indexes */
        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        indexes->push_back(index + 3);
        indexes->push_back(index + 4);
        indexes->push_back(index + 6);

        indexes->push_back(index + 3);
        indexes->push_back(index + 6);
        indexes->push_back(index + 5);

        indexes->push_back(index + 7);
        indexes->push_back(index + 8);
        indexes->push_back(index + 9);

        index += 10;
    }

    return vertices;

}
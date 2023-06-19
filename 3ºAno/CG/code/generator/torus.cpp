#define _USE_MATH_DEFINES
#include <math.h>
#include "torus.hpp"
#include "../shared/point.hpp"

/**
 * Generate a torus with the given parameters
 * @param innerRadius
 * @param outerRadius
 * @param slices
 * @param stacks
 * @param indexes
 * @param normals
 * @param textCoords
 * @return
 */
std::vector<float> generateTorus(float innerRadius, float outerRadius, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoords){
    std::vector<float> vertices;

    // A torus is generated, translating each slice of a sphere centered in the origin to outside

    float alpha = 2 * (float) M_PI / (float) slices; // Defines the position around the y-axis
    float beta = 2 * (float) M_PI / (float) stacks; // Defines the height
    float radius = (outerRadius - innerRadius) / 2;
    float distanceToOrigin = innerRadius + radius;
    float deltaX = 1.0f / slices;
    float deltaY = 1.0f / stacks;
    int index = 0;

    for(int i=0; i < slices; i++){ // Percorrer as slices

        // Center of sphere perimeter
        Point c1(distanceToOrigin * sin((float) i * alpha), 0, distanceToOrigin * cos((float) i * alpha));
        Point c2(distanceToOrigin * sin((float) (i + 1) * alpha), 0, distanceToOrigin * cos((float) (i + 1) * alpha));

        // Points in the center
        Point p3 (radius * sin((float) i * alpha), 0, radius * cos((float) i * alpha));
        Point p4 (radius * sin((float) (i + 1) * alpha), 0, radius * cos((float) (i + 1) * alpha));

        // Displace the p3 and p4 to c1 and c2
        vertices.push_back(c1.getX() + p3.getX());
        vertices.push_back(c1.getY() + p3.getY());
        vertices.push_back(c1.getZ() + p3.getZ());
        normals->push_back(sin((float) i * alpha));
        normals->push_back(0);
        normals->push_back(cos((float) i * alpha));
        textCoords->push_back(i * deltaX);
        textCoords->push_back(0);

        vertices.push_back(c2.getX() + p4.getX());
        vertices.push_back(c2.getY() + p4.getY());
        vertices.push_back(c2.getZ() + p4.getZ());
        normals->push_back(sin((float) (i + 1) * alpha));
        normals->push_back(0);
        normals->push_back(cos((float) (i + 1) * alpha));
        textCoords->push_back((i + 1) * deltaX);
        textCoords->push_back(0);


        for(int j = 0; j < stacks; j++){
            // Update the point in the center sphere
            Point p1 (radius * cos((float) (j + 1) * beta) * sin((float) i * alpha), radius * sin((float) (j + 1) * beta), radius * cos((float) (j + 1) * beta) * cos((float) i * alpha));
            Point p2 (radius * cos((float) (j + 1) * beta) * sin((float) (i + 1) * alpha), radius * sin((float) (j + 1) * beta), radius * cos((float) (j + 1) * beta) * cos((float) (i + 1) * alpha));

            // Translate that point
            vertices.push_back(c1.getX() + p1.getX());
            vertices.push_back(c1.getY() + p1.getY());
            vertices.push_back(c1.getZ() + p1.getZ());
            normals->push_back(cos((float) (j + 1) * beta) * sin((float) i * alpha));
            normals->push_back(sin((float) (j + 1) * beta));
            normals->push_back(cos((float) (j + 1) * beta) * cos((float) i * alpha));
            textCoords->push_back(i * deltaX);
            textCoords->push_back((j + 1) * deltaY);

            vertices.push_back(c2.getX() + p2.getX());
            vertices.push_back(c2.getY() + p2.getY());
            vertices.push_back(c2.getZ() + p2.getZ());
            normals->push_back(cos((float) (j + 1) * beta) * sin((float) (i + 1) * alpha));
            normals->push_back(sin((float) (j + 1) * beta));
            normals->push_back(cos((float) (j + 1) * beta) * cos((float) (i + 1) * alpha));
            textCoords->push_back((i + 1) * deltaX);
            textCoords->push_back((j + 1) * deltaY);

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        index += 2;
    }

    return vertices;
}
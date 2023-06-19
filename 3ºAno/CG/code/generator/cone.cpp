#define _USE_MATH_DEFINES
#include <math.h>
#include "cone.hpp"
#include "../shared/point.hpp"

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
std::vector<float> generateCone(float radius, float height, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoords) {
    std::vector<float> vertices;
    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = height / (float) stacks;
    float radiusDec = radius / (float) stacks;
    float deltaX = 1.0f / slices;
    float deltaY = 1.0f / stacks;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        /* Base of the cone */

        // Center of the base
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        normals->push_back(0);
        normals->push_back(-1); // Normal is pointing downwards
        normals->push_back(0);
        textCoords->push_back(0.5f);
        textCoords->push_back(0.5f);

        // Point in the perimeter of the base
        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));
        normals->push_back(0);
        normals->push_back(-1); // Normal is still pointing downwards
        normals->push_back(0);
        textCoords->push_back(0.5f + 0.5 * sin(i * sliceStep)); // We inscribe the polygon of the
        textCoords->push_back(0.5f + 0.5 * cos(i * sliceStep)); // base in a circumference of radius 0.5
                                                                   // inside the texture image

        // The other point to make a triangle
        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        textCoords->push_back(0.5f + 0.5 * sin((i + 1) * sliceStep));
        textCoords->push_back(0.5f + 0.5 * cos((i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        // Points to calculate the normal for each geratrix
        Point p1(radius * sin((float) (i-1) * sliceStep), 0, radius * cos((float) (i-1) * sliceStep));
        Point p2(radius * sin((float) (i+1) * sliceStep), 0, radius * cos((float) (i+1) * sliceStep));
        Point p3(radius * sin((float) i * sliceStep), 0, radius * cos((float) i * sliceStep));
        Point p4(0, height, 0);

        // Vectors to calculate the normal for each geratrix
        Point v1(p2.getX()-p1.getX(), p2.getY()-p1.getY(), p2.getZ()-p1.getZ());
        Point v2(p4.getX()-p3.getX(), p4.getY()-p3.getY(), p4.getZ()-p3.getZ());

        // Normal in i
        Point n1 = Point::crossProduct(v1, v2);
        n1.normalize();

        // Same thing
        p1.setPoint(radius * sin((float) i * sliceStep), 0, radius * cos((float) i * sliceStep));
        p2.setPoint(radius * sin((float) (i+2) * sliceStep), 0, radius * cos((float) (i+2) * sliceStep));
        p3.setPoint(radius * sin((float) (i+1) * sliceStep), 0, radius * cos((float) (i+1) * sliceStep));

        v1.setPoint(p2.getX()-p1.getX(), p2.getY()-p1.getY(), p2.getZ()-p1.getZ());
        v2.setPoint(p4.getX()-p3.getX(), p4.getY()-p3.getY(), p4.getZ()-p3.getZ());

        // Normal in i+1
        Point n2 = Point::crossProduct(v1, v2);
        n2.normalize();

        index += 3;

        // Again, the two vertices in the base, but for the slice
        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));
        normals->push_back(n1.getX());
        normals->push_back(n1.getY()); // Put the normals calculated before
        normals->push_back(n1.getZ());
        textCoords->push_back(i * deltaX);
        textCoords->push_back(0);

        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));
        normals->push_back(n2.getX());
        normals->push_back(n2.getY()); // Put the normals calculated before
        normals->push_back(n2.getZ());
        textCoords->push_back((i + 1) * deltaX); // Division of the texture into multiple slices (bottom of the image because of stack 0)
        textCoords->push_back(0);

        for (int j = 0; j < stacks - 1; j++) {
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) i * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) i * sliceStep));
            normals->push_back(n1.getX());
            normals->push_back(n1.getY()); // The normals are already calculated
            normals->push_back(n1.getZ()); // and are the same for all geratrix
            textCoords->push_back(i * deltaX); // We are now in the (j+1) stack
            textCoords->push_back((j+1) * deltaY);

            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) (i + 1) * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) (i + 1) * sliceStep));
            normals->push_back(n2.getX());
            normals->push_back(n2.getY());
            normals->push_back(n2.getZ());
            textCoords->push_back((i+1) * deltaX);
            textCoords->push_back((j+1) * deltaY);

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        vertices.push_back(0);
        vertices.push_back(height);
        vertices.push_back(0);
        normals->push_back(0);
        normals->push_back(1); // Normal pointing upwards
        normals->push_back(0);
        textCoords->push_back((i+0.5f) * deltaX); // There are parts of the texture not used since the cone ends in one vertex
        textCoords->push_back(1);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        index += 3;
    }

    return vertices;
}
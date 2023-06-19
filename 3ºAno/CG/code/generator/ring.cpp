#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "ring.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include <string>

/**
 * Generates n random objects inside a ring
 * @param outerRadius
 * @param innerRadius
 * @param n
 * @param minScale
 * @param maxScale
 * @param minAngle
 * @param maxAngle
 * @param indexes
 * @param normals
 * @param textCoord
 * @param args
 * @return
 */
vector<float> generateRing (float outerRadius, float innerRadius, int n, float minScale, float maxScale, float minAngle, float maxAngle, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoord, char** args){
    vector<float> vertices;
    vector<float> aux;
    int index = 0;

    // For each object to be generated in a ring
    for(int j = 0; j < n; j++) {

        float angle = (float) ((float) rand() / (float) RAND_MAX) * 2 * (float) M_PI; // Pseudo-random angle between 0 and 360º
        float verticalAngle = ((float) rand() / (float) RAND_MAX) * (maxAngle - minAngle) +
                              minAngle; // Pseudo-random angle between 0 and 360º
        verticalAngle = verticalAngle * (float) M_PI / 180;
        float distance = ((float) rand() / (float) RAND_MAX) * (outerRadius - innerRadius) + innerRadius;
        float scaleF = ((float) rand() / (float) RAND_MAX) * (maxScale - minScale) + minScale;

        Point start(distance * cos(verticalAngle) * sin(angle), distance * sin(verticalAngle), distance * cos(verticalAngle) * cos(angle));

        // Only possible to generate spheres or boxes
        if (strcmp(args[0], "sphere") == 0) {
            aux = generateSphere(scaleF, stoi(args[1]), stoi(args[2]), start, &index, indexes, normals, textCoord);
        } else if (strcmp(args[0], "box") == 0) {
            aux = generateBox(scaleF, stoi(args[1]), start, indexes, &index, normals, textCoord);
        }
        vertices.insert(vertices.end(), aux.begin(), aux.end());
    }

    return vertices;
}

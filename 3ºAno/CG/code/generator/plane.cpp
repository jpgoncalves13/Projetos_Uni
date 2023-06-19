#include "plane.hpp"

/**
 * Generates a group of triangles that combine into a grid, making a square (plane) of given length
 * @param length the length of the larger square
 * @param grid number of smaller squares per side
 * @param direction defines the direction of the plane (for each coordinate, 0 - no direction)
 * For example, 0 in the y direction means the plane is parallel to the y=0 plane
 * @param initial the plane to start generating the plane
 * @param clockWiseDir true if direction is set to clockwise, false if direction is set to counterclockwise
 * @return a list of generated triangles
 */
vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index, vector<float>* normals, vector<float>* textCoord){
    vector<float> vertices;
    float step = length / (float) grid; // side of each of the smaller squares

    Point base (initial.getX(), initial.getY(), initial.getZ());
    Point normal;

    // Normals depending on the plane they are parallel to and the direction
    if(direction.getY() == 0 && !clockWiseDir) {
        normal.setPoint(0,1,0);
    } else if (direction.getY() == 0) {
        normal.setPoint(0,-1,0);
    } else if (direction.getX() == 0 && !clockWiseDir) {
        normal.setPoint(1,0,0);
    } else if (direction.getX() == 0) {
        normal.setPoint(-1,0,0);
    } else if (direction.getZ() == 0 && !clockWiseDir) {
        normal.setPoint(0,0,1);
    } else if (direction.getZ() == 0) {
        normal.setPoint(0,0,-1);
    }

    float textInc = 1.0f/(float)grid;
    // Gerado a partir do canto superior esquerdo, depois coluna a coluna
    for(int i=0; i<grid; i++) {
        vertices.push_back(base.getX());
        vertices.push_back(base.getY());
        vertices.push_back(base.getZ());
        normals->push_back(normal.getX());
        normals->push_back(normal.getY());
        normals->push_back(normal.getZ());
        textCoord->push_back(textInc*i);
        textCoord->push_back(1);

        // Next point calculated basing on the X direction
        // If x direction is 0 -> step is in the Z direction
        if (direction.getX() == 0) {
            vertices.push_back(base.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
            normals->push_back(normal.getX());
            normals->push_back(normal.getY());
            normals->push_back(normal.getZ());
        }
        // If x direction is not 0 -> step is in the X direction
        else {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
            normals->push_back(normal.getX());
            normals->push_back(normal.getY());
            normals->push_back(normal.getZ());
        }


        textCoord->push_back(textInc*(i+1));
        textCoord->push_back(1);

        for (int j = 0; j < grid; j++) {
            // Next point calculated based on the Y direction
            // If y direction is 0 -> step is in the Z direction
            if (direction.getY() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY());
                vertices.push_back(base.getZ() + step * direction.getZ());
                normals->push_back(normal.getX());
                normals->push_back(normal.getY());
                normals->push_back(normal.getZ());
            }
                // If y direction is not 0 -> step is in the Y direction
            else {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
                normals->push_back(normal.getX());
                normals->push_back(normal.getY());
                normals->push_back(normal.getZ());
            }

            textCoord->push_back(textInc*i);
            textCoord->push_back(1-textInc*(j+1));

            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY() + step * direction.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
            normals->push_back(normal.getX());
            normals->push_back(normal.getY());
            normals->push_back(normal.getZ());
            textCoord->push_back(textInc*(i+1));
            textCoord->push_back(1-textInc*(j+1));

            // Calculate the order based on the clockWiseDir
            if (direction.getZ() == 0 && !clockWiseDir) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 1);

                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 3);
            } else if (direction.getZ() == 0) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 2);

                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 2);
            } else if (!clockWiseDir) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 3);

                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 1);
            } else {
                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 2);

                indexes->push_back(*index);
                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 3);
            }

            // Move the base point
            if (direction.getY() == 0) {
                base.setZ(base.getZ() + step * direction.getZ());
            }
            else {
                base.setY(base.getY() + step * direction.getY());
            }

            (*index) += 2;
        }
        // When ends that row, must go to the next row
        if (direction.getX() == 0) {
            base.setZ(base.getZ() + step * direction.getZ());
            base.setY(initial.getY());
        } else if (direction.getY() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setZ(initial.getZ());
        } else if (direction.getZ() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setY(initial.getY());
        }

        (*index) += 2;
    }
    return vertices;
}
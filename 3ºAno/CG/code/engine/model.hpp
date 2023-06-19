/**
 * A Model is a representation of an object in the world
 * It has:
 *  - a label;
 *  - color;
 *  - vertices and indexes;
 */

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include <string>
#include <tuple>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <map>

#endif
#include "../shared/point.hpp"
#include "BoundingVolumes/volume.hpp"
#include "camera.hpp"
#include "BoundingVolumes/plane.hpp"

using namespace std;

class Model {
private:
    string label;
    GLuint vertices;
    GLuint indexes;
    GLuint normals;
    GLuint texCoord;
    int indexCount;
    Volume *v;
    // Material colors
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    GLuint texId;
    string textureFile;

public:
    Model();
    void readModel(string fileName);
    int drawModel(bool vboActive, Plane *planes, float *matrix, map<string, tuple<Point, float>> *teleports, bool mipmap, bool change);
    string getLabel();
    void setLabel(string label);
    Point getDiffuse();
    void setDiffuse(Point diffuse);
    Point getAmbient();
    void setAmbient(Point ambient);
    Point getSpecular();
    void setSpecular(Point specular);
    Point getEmissive();
    void setEmissive(Point emissive);
    float getShininess();
    void setShininess(float shininess);
    string getTextureFile();
    void setTextureFile(string textureFile);
    void setBVParamsIfSphere(float *matrix);
private:
    void loadImage(string textFile, bool mipmap);
};


#endif //ENGINE_MODEL_H

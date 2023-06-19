#include "model.hpp"
#include "../shared/IO.hpp" // reader function
#include "../shared/matrixOp.hpp"
#include "../shared/point.hpp"
#include "BoundingVolumes/volume.hpp"
#include "BoundingVolumes/sphere.hpp"
#include "camera.hpp"
#include "BoundingVolumes/plane.hpp"
#include <map>
#include <cmath>
#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

float diffuse[4] = {};
float ambient[4] = {};
float specular[4] = {};
float emissive[4] = {};
float shininess = 0;

/**
 * Sets the default materials for the object
 */
Model::Model() {
    this->texId = 0;
    this->diffuse[0] = 200.0f / 255.0f;
    this->diffuse[1] = 200.0f / 255.0f;
    this->diffuse[2] = 200.0f / 255.0f;
    this->diffuse[3] = 1;

    this->ambient[0] = 50.0f / 255.0f;
    this->ambient[1] = 50.0f / 255.0f;
    this->ambient[2] = 50.0f / 255.0f;
    this->ambient[3] = 1;

    this->specular[0] = 0;
    this->specular[1] = 0;
    this->specular[2] = 0;
    this->specular[3] = 1;

    this->emissive[0] = 0;
    this->emissive[1] = 0;
    this->emissive[2] = 0;
    this->emissive[3] = 1;

    this->shininess = 0;
    this->v = nullptr;
}

/**
 * Renders the model
 * @param vboActive true if we want to render it using vbos
 * @param planes vfc planes
 * @param matrix
 * @param teleports map with the teleports
 * @param mipmap if mipmap is true
 * @param change if texture filters have changed -> we want to reload the texture
 * @return
 */
int Model::drawModel(bool vboActive, Plane *planes, float *matrix, map<string, tuple<Point, float>> *teleports, bool mipmap, bool change) {
    // Setup teleports
    if (this->label.compare("undefined") != 0) {
        Point p1(matrix[3], matrix[7], matrix[11]);
        float res[4];
        float p[4] = {2,2,2,1};
        multiplyMatrixVector(matrix, p, res);
        float radius = sqrt((res[0]-matrix[3])*(res[0]-matrix[3]) + (res[1]-matrix[7])*(res[1]-matrix[7]) + (res[2]-matrix[11])*(res[2]-matrix[11]));
        (*teleports)[this->label] = make_tuple(p1, radius);
    }

    if (change)// ReLoad the texture
        this->loadImage(this->getTextureFile(), mipmap);

    // Setup materials
    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, this->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, this->shininess);

    int ret = 0;
    if (this->v == nullptr || this->v->test(planes, matrix)) { // Test VFC
        glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, this->normals);
        glNormalPointer(GL_FLOAT, 0, 0);

        // If texture is active
        if (texId != 0) {
            glBindTexture(GL_TEXTURE_2D, texId);
            glBindBuffer(GL_ARRAY_BUFFER, this->texCoord);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
        glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        ret = this->indexCount / 3;
    }

    return ret;
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
void Model::readModel(string fileName) {
    vector<float> textCoordsVector;
    vector<float> normalsVector;
    vector<unsigned int> indexesVector;

    vector<float> verticesVector = reader(fileName, &indexesVector, &normalsVector, &textCoordsVector, &this->v);

    glGenBuffers(1, &(this->vertices));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVector.size(), verticesVector.data(),GL_STATIC_DRAW);

    glGenBuffers(1, &(this->indexes));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * indexesVector.size(), indexesVector.data(),GL_STATIC_DRAW);

    glGenBuffers(1, &(this->normals));
    glBindBuffer(GL_ARRAY_BUFFER,this->normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsVector.size() , normalsVector.data(),GL_STATIC_DRAW);
    
    if (this->texId!=0) {
        glGenBuffers(1, &(this->texCoord));
        glBindBuffer(GL_ARRAY_BUFFER, this->texCoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textCoordsVector.size(), textCoordsVector.data(), GL_STATIC_DRAW);
    }

    this->indexCount = indexesVector.size();
}

/**
 * Set the label for a model
 * @param label the label
 */
void Model::setLabel(string label) {
    this->label = label;
}

// Set the multiple materials available to the object

void Model::setDiffuse(Point diffuse) {
    this->diffuse[0] = diffuse.getX() / 255;
    this->diffuse[1] = diffuse.getY() / 255;
    this->diffuse[2] = diffuse.getZ() / 255;
}

void Model::setAmbient(Point ambient) {
    this->ambient[0] = ambient.getX() / 255;
    this->ambient[1] = ambient.getY() / 255;
    this->ambient[2] = ambient.getZ() / 255;
}

void Model::setSpecular(Point specular) {
    this->specular[0] = specular.getX() / 255;
    this->specular[1] = specular.getY() / 255;
    this->specular[2] = specular.getZ() / 255;
}

void Model::setEmissive(Point emissive) {
    this->emissive[0] = emissive.getX() / 255;
    this->emissive[1] = emissive.getY() / 255;
    this->emissive[2] = emissive.getZ() / 255;
}

Point Model::getDiffuse() {
    return Point(this->diffuse[0], this->diffuse[1], this->diffuse[2]);
}

Point Model::getAmbient() {
    return Point(this->ambient[0], this->ambient[1], this->ambient[2]);
}

Point Model::getSpecular() {
    return Point(this->specular[0], this->specular[1], this->specular[2]);
}

float Model::getShininess() {
    return shininess;
}

void Model::setShininess(float shininess) {
    this->shininess = shininess;
}

// Set the texture file (we may want to reload it after)
void Model::setTextureFile(string textureFile) {
    loadImage(textureFile,false);
    this->textureFile = textureFile;
}

void Model::setBVParamsIfSphere(float *matrix) {
    Sphere* sv = dynamic_cast<Sphere*>(this->v);
    if (sv) {
        float res[4];
        float p[4] = { sv->getCenter().getX(),sv->getCenter().getY(),sv->getCenter().getZ(),1};
        multiplyMatrixVector(matrix, p, res);
        Point p1(res[0], res[1], res[2]);
        sv->setCenter(p1);

        float scaleX = std::sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
        sv->setRadius(sv->getRadius() * scaleX);
    }
}

/**
 * Load image as a texture
 * @param textFile
 * @param mipmap
 */
void Model::loadImage(string textFile, bool mipmap) {
    unsigned int t,tw,th;
    unsigned char *texData;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)textFile.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1,&this->texId);

    glBindTexture(GL_TEXTURE_2D,texId);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
    // Multiple config filter params
    if (mipmap){
        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else{
        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Get the path of the texture
string Model::getTextureFile() {
    return this->textureFile;
}



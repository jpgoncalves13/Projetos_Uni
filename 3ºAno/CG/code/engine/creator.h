#ifndef ENGINE_CREATOR_H
#define ENGINE_CREATOR_H

#include <tuple>
#include <vector>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include "camera.hpp"
#include "Lights/Light.h"

#endif

class Creator {
private:
    int red = 0, green = 0, blue = 0;
    int indColors = 0;

    std::vector<std::tuple<float, float, float>> colorsVec; // Vector with colors available to the user - Disabled
    std::vector<int> textureVec; // Vector with textures available to the user

    GLuint buffers[5]; // VBos

    // Mapping from position to cube index
    std::map<std::tuple<float, float, float>, unsigned int> mapPointIndex;

    // Vectors to store data initially
    std::vector<float> vertices;
    std::vector<unsigned int> indexes;
    std::vector<float> colors;
    std::vector<float> normalColors;

    std::vector<float> cubesPositions;
    std::vector<float> cubesColors;
    std::vector<float> normals;
    std::vector<float> textureCoords;

    float nextPosX[255][255][255];
    float nextPosY[255][255][255];
    float nextPosZ[255][255][255];

    float currPosX[255][255][255];
    float currPosY[255][255][255];
    float currPosZ[255][255][255];

    int vertexCount = 0, indexCount = 0, numIndex = 0, cubeIndex = 0;

    Camera* globalCamera;
    Light l;
    GLuint texIds[100];
    int textInd=0;
    std::vector<string> textureNames; // Names of the textures (to be displayed on the screen)
    int numTextures; // Number of textures loaded
    std::vector<std::tuple<GLuint, int>> blockTextures; // Vector that tells the texture attributed to each portion of the arrays

    // Current texture to be rendered
    GLuint currentTextureTop;
    GLuint currentTextureSides;
public:
    Creator(Camera* camera);
    ~Creator();
    void drawCube(int x, int y, int z, float cRed, float cGreen, float cBlue, bool addTexture);
    void addCube(int x, int y, int z);
    void removeCube(unsigned int index);
    void render(int height, int width);
    void processMouseButtons(int button, int state, int xx, int yy);
    unsigned char* picking(int x, int y);
    void changeBlockColor();
    void changeBlockTexture();
    void importScene(string filename);
    void exportScene(string filename);
private:
    void incrementColors();
    void renderText(int height, int width);
    void loadTexture(string path);
};


#endif //ENGINE_CREATOR_H

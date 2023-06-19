#include "creator.h"
#include "Lights/LightPoint.h"
#include "Lights/LightDirectional.h"

#include <IL/il.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
//#include <unistd.h>


/////// MINECRAFT /////////

unsigned char* imageData;

unsigned int t, tw, th;

/**
 * Imports a scene from a file in the filesystem
 * The file contains the positions of the blocks
 * @param filename The name of the file
 */
void Creator::importScene(string filename) {
    ifstream file("../../scenes/" + filename);

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << filename << endl;
    }
    else {
        string line;
        int mode = 0;
        while (getline(file, line, '\n')) {

            // [vf] \d+ \d+ \d+
            vector<string> strings;
            stringstream ss(line);
            string word;
            while (ss >> word) {
                strings.push_back(word);
            }
            if (mode == 0){
                if (strings.size() == 1 && strings[0].compare("v") == 0){
                    mode = 1;
                }
                else
                    this->blockTextures.push_back(make_tuple(stoi(strings[0]), stoi(strings[1])));
            }
            else if (mode == 1){
                float xCoord = stof(strings[0]);
                float yCoord = stof(strings[1]);
                float zCoord = stof(strings[2]);

                float redComp = 0;
                float greenComp = 0;
                float blueComp = 0;

                drawCube(xCoord, yCoord, zCoord, redComp, greenComp, blueComp, false);
                vertexCount += 6 * 4; //
            }

        }
    }
}

/**
 * Imports a scene to a file in the filesystem
 * The file contains the positions of the blocks
 * @param filename The name of the file
 */
void Creator::exportScene(std::string filename) {
    ofstream file("../../scenes/" + filename);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << filename << endl;
    }
    else
    {
        for(std::tuple<GLuint, int> tuple: this->blockTextures){
            GLuint ind = std::get<0>(tuple);
            int numberInd = std::get<1>(tuple);
            file << ind << " " << numberInd << endl;
        }
        file << "v" << endl;
        // Each block is described in the file
        for(int i=0; i<cubesPositions.size(); i+=3){
            // IF the block has not been removed
            if (cubesPositions[i] != 1000000 && cubesPositions[i+1] != 1000000 && cubesPositions[i+2] != 1000000){
                file << cubesPositions[i] << " " << cubesPositions[i + 1] << " " << cubesPositions[i + 2] << endl;
            }

        }
    }
}

/**
 * Used to change the selected color: DEPRECATED
 */
void Creator::changeBlockColor(){
    indColors = (indColors+1)%colorsVec.size();
    std::cout << "Color: " << indColors << std::endl;
    std::cout << get<0>(colorsVec[indColors]) << std::endl;
}

/**
 * Used to change the current texture in use
 */
void Creator::changeBlockTexture(){
    textInd = (textInd+1)%numTextures;
    switch(textInd){
        case 0: // Grass
            currentTextureTop = 1;
            currentTextureSides = 0;
            break;
        case 1: // Birch Log
            currentTextureTop = 3;
            currentTextureSides = 2;
            break;
        case 2: // Birch Planks
            currentTextureTop = 4;
            currentTextureSides = 4;
            break;
        case 3: // Bricks
            currentTextureTop = 5;
            currentTextureSides = 5;
            break;
        case 4: // Coarse dirt
            currentTextureTop = 6;
            currentTextureSides = 6;
            break;
        case 5: // Diamond block
            currentTextureTop = 7;
            currentTextureSides = 7;
            break;
        case 6: // Oak Log
            currentTextureTop = 9;
            currentTextureSides = 8;
            break;
        case 7: // Redstone lamp
            currentTextureTop = 11;
            currentTextureSides = 11;
            break;
        case 8: // Redstone lamp on
            currentTextureTop = 12;
            currentTextureSides = 12;
            break;
        case 9: // Sand
            currentTextureTop = 13;
            currentTextureSides = 13;
            break;
        case 10: // Leaves
            currentTextureTop = 14;
            currentTextureSides = 14;
            break;
        case 11: // Cobblestone
            currentTextureTop = 15;
            currentTextureSides = 15;
            break;
        case 12:
            currentTextureTop = 16;
            currentTextureSides = 16;
            break;
    }
}

/**
 * Get the height of a position in a height map
 * @param i the x component of the image
 * @param j the y component of the image
 * @return the height in that position
 */
float getHeight(int i, int j) {
    return imageData[i * tw + j];
}

/**
 * Creator initializing function
 * Initializes:
 * - vbos
 * - textures
 * - camera
 * - lights
 * - basic setup of worlds, according to choice of user
 * @param camera the camera from the engine
 */
Creator::Creator(Camera* camera) {
    std::cout << "Como deseja inicializar o terreno?\n 1 - Plano\n 2 - Ficheiro de Imagem\n 3 - Importar terreno" << std::endl;
    int option;
    std::cin >> option;

    this->globalCamera = camera;
    ilInit();

    colorsVec.push_back(std::make_tuple(1, 0, 0)); // RED
    colorsVec.push_back(std::make_tuple(0, 1, 0)); // GREEN
    colorsVec.push_back(std::make_tuple(0, 0, 1)); // BLUE
    colorsVec.push_back(std::make_tuple(1, 1, 0.1)); // LASER LEMON
    colorsVec.push_back(std::make_tuple(1, 0.3, 1)); // ULTRA PINK
    colorsVec.push_back(std::make_tuple(0, 0, 0)); // BLACK
    colorsVec.push_back(std::make_tuple(1, 1, 1)); // WHITE
    colorsVec.push_back(std::make_tuple(0, 0.2, 0)); // DARK GREEN
    colorsVec.push_back(std::make_tuple(0.2, 0.07, 0)); // ZINNWALDITE BROWN

    // 5 buffers - vertex, indexes, normals, colors (picking), textures
    glGenBuffers(5, buffers);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY); Replaced by textures
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_2D);
    string path = "../Textures/minecraft_textures/";
    loadTexture(path + "grass_block_side.png");
    loadTexture(path + "Moss_block.jpg");
    loadTexture(path + "Birch_log.jpg");
    loadTexture(path + "Birch_log_top.jpg");
    loadTexture(path + "Birch_planks.jpg");
    loadTexture(path + "Bricks_top.jpg");
    loadTexture(path + "Coarse_dirt.jpg");
    loadTexture(path + "Diamond_block.jpg");
    loadTexture(path + "Oak_log.jpg");
    loadTexture(path + "Oak_log_top.jpg");
    loadTexture(path + "Oak_planks.jpg");
    loadTexture(path + "redstone_lamp.jpg");
    loadTexture(path + "redstone_lamp_on.jpg");
    loadTexture(path + "Sand_top.jpg");
    loadTexture(path + "Azalea_leaves.jpg");
    loadTexture(path + "Cobblestone.jpg");
    loadTexture(path + "EmeraldOre.jpg");
    numTextures = 13;

    textureNames.push_back("GRASS");
    textureNames.push_back("BIRCH LOG");
    textureNames.push_back("BIRCH PLANKS");
    textureNames.push_back("BRICKS");
    textureNames.push_back("COARSE DIRT");
    textureNames.push_back("DIAMOND BLOCK");
    textureNames.push_back("OAK LOG");
    textureNames.push_back("REDSTONE LAMP OFF");
    textureNames.push_back("REDSTONE LAMP ON");
    textureNames.push_back("SAND");
    textureNames.push_back("LEAVES");
    textureNames.push_back("COBBLESTONE");
    textureNames.push_back("EMERALD_ORE");

    currentTextureSides = 0; // Grass
    currentTextureTop = 1; // Grass

    textInd = 0;

    if (option == 1){
        std::cout << "Qual o tamanho do plano? " << std::endl;
        int side;
        std::cin >> side;
        for (int i = 0; i < side; i++) {
            for (int j = 0; j < side; j++) {
                drawCube(i, 0, j, 0, 1, 0, true);
            }
        }

        vertexCount += side * side * 6 * 4;
    }
    else if (option == 2) {
        // 	Load the height map "terreno.jpg"
        ilGenImages(1, &t);
        ilBindImage(t);

        // terreno.jpg is the image containing our height map
        ilLoadImage((ILstring) "../terreno.jpg");

        // convert the image to single channel per pixel
        // with values ranging between 0 and 255
        ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

        // important: check tw and th values
        // both should be equal to 256
        // if not there was an error loading the image
        // most likely the image could not be found
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        std::cout << "Largura: " << tw << " Altura: " << th << std::endl;

        // imageData is a LINEAR array with the pixel values
        imageData = ilGetData();

        int h = (int) th - 1, w = (int) tw - 1;

        int side = h;
        int cubeHeight = 1;

        for (int i = 0+50; i < h-50; i++) {
            for (int j = 0+50; j < w-50; j++) {
                //drawCube(i, getHeight(i, j) / 2, j, -1, -1, -1);
                int height = (int)getHeight(i,j)/2;
                float colorRed=-1, colorGreen=-1, colorBlue=-1;

                for(int k=height-cubeHeight; k < height+1; k++) {
                    if (k == 0) {
                        colorRed = 1;
                        colorBlue = 0;
                        colorGreen = 0;
                    }
                    drawCube(i - w / 2, k, j - h / 2, colorRed, colorGreen, colorBlue, true);
                }
            }
        }

        vertexCount += side * side * (cubeHeight+1) * 6 * 4;
    }
    else if (option == 3){
        std::cout << "Qual o nome do ficheiro a importar? " << std::endl;
        string filename;
        std::cin >> filename;

        this->importScene(filename);
        currentTextureSides = 0; // Grass
        currentTextureTop = 1; // Grass

        textInd = 0;
    }

    int maxPlacedBlocks = 10000;

    // We reserve some memory for blocks placed in the future by filling each vbo with 0s
    for (int i = 0; i < maxPlacedBlocks; i++) {
        for (int j = 0; j < 24; j++) {
            vertices.push_back(0);
            colors.push_back(0);
            normalColors.push_back(0);
            normals.push_back(0);
            if (j % 3 == 0 || j % 3 == 1){ // 3:2
                textureCoords.push_back(0);
            }
        }

        for (int k = 0; k < 12; k++) {
            indexes.push_back(0);
        }
    }

    l = LightPoint(0,-100,0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int), indexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, buffers[3]); // Not used anymore
    //glBufferData(GL_ARRAY_BUFFER, normalColors.size() * sizeof(float), normalColors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), textureCoords.data(), GL_STATIC_DRAW);
}

/**
 * Destructor of creator mode
 * Prompts the user to save progress
 */
Creator::~Creator() {
    std::cout << "Deseja guardar o resultado em ficheiro? S | N" << std::endl;
    char option;
    std::cin >> option;

    if (option == 'S'){
        std::cout << "Introduza o nome do ficheiro ... " << std::endl;
        string filename;
        std::cin >> filename;

        this->exportScene(filename);
    }
}

/**
 * Renders the selected texture to be placed
 * @param height height of screen
 * @param width width of screen
 */
void Creator::renderText(int height, int width) {
    // Save the current matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // set projection so that coordinates match window pixels
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glLoadIdentity();
    glRasterPos2d(0.0f*(float)width, 0*(float)height); // text position in pixels - bottom left corner

    glColor3f(1,1,1);
    const char* c = textureNames[textInd].c_str(); // Display the selected texture
    for (; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

/**
 * Used to update the color the block is colored in the background - not visible but essential to picking algorithm
 */
void Creator::incrementColors() {
    red += 1;
    if (red >= 255) {
        red = 0;
        green++;
        if (green == 255) {
            green = 0;
            blue++;
        }
    }
}

/**
 * Place a cube in the world
 * @param x position x of the cube
 * @param y position y of the cube
 * @param z position z of the cube
 * @param cRed red component of the cube (not used anymore)
 * @param cGreen green component of the cube (not used anymore)
 * @param cBlue blue component of the cube (not used anymore)
 */
void Creator::drawCube(int x, int y, int z, float cRed, float cGreen, float cBlue, bool addTexture) {
    float newVertices[] = {
            -0.5f + x, -0.5f + y, 0.5f + z, // FRONT
            0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BACK
            -0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z, // TOP
            -0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BOTTOM
            0.5f + x, -0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z, // RIGHT
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // LEFT
            -0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z
    };

    if (addTexture){
        // Possible otimization: reorder faces  reducing the calls to glDraw
        this->blockTextures.push_back(make_tuple(currentTextureSides, 12));
        this->blockTextures.push_back(make_tuple(currentTextureTop, 12));
        this->blockTextures.push_back(make_tuple(currentTextureSides, 12));
    }


    float textCoords[] = {
            0, 0, // FRONT
            1, 0,
            1, 1,
            0, 1,
            1, 0, // BACK
            1, 1,
            0, 1,
            0, 0,
            0,1, // TOP
            0,0,
            1,0,
            1,1,
            0,0, // BOTTOM
            1,0,
            1,1,
            0,1,
            1,0, // RIGHT
            1,1,
            0,1,
            0,0,
            0,0, // LEFT
            1,0,
            1,1,
            0,1
    };

    float normals[] = {
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0
    };

    int newIndexes[] = {
            indexCount + 0, indexCount + 1, indexCount + 2, // FRONT
            indexCount + 2, indexCount + 3, indexCount + 0,
            indexCount + 0+4, indexCount + 1+4, indexCount + 2+4, // BACK
            indexCount + 2+4, indexCount + 3+4, indexCount + 0+4,
            indexCount + 0+8, indexCount + 1+8, indexCount + 2+8, // TOP
            indexCount + 2+8, indexCount + 3+8, indexCount + 0+8,
            indexCount + 0+12, indexCount + 1+12, indexCount + 2+12, // BOTTOM
            indexCount + 2+12, indexCount + 3+12, indexCount + 0+12,
            indexCount + 0+16, indexCount + 1+16, indexCount + 2+16, // RIGHT
            indexCount + 2+16, indexCount + 3+16, indexCount + 0+16,
            indexCount + 0+20, indexCount + 1+20, indexCount + 2+20, // LEFT
            indexCount + 2+20, indexCount + 3+20, indexCount + 0+20,
    }; // Foreach face, the indexCount is increment 4 unities

    indexCount += 24;

    float red_vec = std::get<0>(colorsVec[indColors]);
    float green_vec = std::get<1>(colorsVec[indColors]);
    float blue_vec = std::get<2>(colorsVec[indColors]);

    float newNormalColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector
    for(int i=0; i<4*6*3; i+=3){
        if (cRed == -1 && cGreen == -1 && cBlue == -1) {
            newNormalColors[i] = red/255.0f;
            newNormalColors[i + 1] = green/255.0f;
            newNormalColors[i + 2] = blue/255.0f;
        }
        else{
            newNormalColors[i] = cRed;
            newNormalColors[i + 1] = cGreen;
            newNormalColors[i + 2] = cBlue;
        }
    } // All vertices with the same color

    float newColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector

    for(int i=0; i<4*6*3; i+=12){
        for(int j=0; j<4; j++){
            newColors[i+3*j] = red / 255.0f;
            newColors[i+3*j+1] = green / 255.0f;
            newColors[i+3*j+2] = blue / 255.0f;
        }

        int nextX = x, nextY = y, nextZ = z;
        // Calculate next block
        switch(i/12){
            case 0: // FRONT
                nextZ = z+1;
                break;
            case 1: // BACK
                nextZ = z-1;
                break;
            case 2: // TOP
                nextY = y+1;
                break;
            case 3: // BOTTOM
                nextY = y-1;
                break;
            case 4: // RIGHT
                nextX = x+1;
                break;
            case 5: // LEFT
                nextX = x-1;
                break;
        }

        nextPosX[red][green][blue] = nextX;
        nextPosY[red][green][blue] = nextY;
        nextPosZ[red][green][blue] = nextZ;

        currPosX[red][green][blue] = x;
        currPosY[red][green][blue] = y;
        currPosZ[red][green][blue] = z;

        incrementColors();
    }

    for(float v: newVertices){
        vertices.push_back(v);
    }

    for(int ind: newIndexes){
        indexes.push_back(ind);
    }

    for(float normalcolor: newNormalColors){
        normalColors.push_back(normalcolor);
    }

    for(float color: newColors){
        colors.push_back(color);
    }

    for(float n: normals){
        this->normals.push_back(n);
    }

    for(float t: textCoords){
        this->textureCoords.push_back(t);
    }

    // Save position of the cube
    cubesPositions.push_back(x);
    cubesPositions.push_back(y);
    cubesPositions.push_back(z);
    cubesColors.push_back(cRed);
    cubesColors.push_back(cGreen);
    cubesColors.push_back(cBlue);

    numIndex += 36;
    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;
    cubeIndex++;
    incrementColors();
}

/**
 * Place a block in the scene (after initial setup)
 * @param x x position
 * @param y y position
 * @param z z position
 */
void Creator::addCube(int x, int y, int z) {
    float newVertices[] = {
            -0.5f + x, -0.5f + y, 0.5f + z, // FRONT
            0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BACK
            -0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z, // TOP
            -0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BOTTOM
            0.5f + x, -0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z, // RIGHT
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // LEFT
            -0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z
    };

    // Otimization possible
    this->blockTextures.push_back(make_tuple(currentTextureSides, 12));
    this->blockTextures.push_back(make_tuple(currentTextureTop, 12));
    this->blockTextures.push_back(make_tuple(currentTextureSides, 12));

    float newTextCoords[] = {
            0, 0, // FRONT
            1, 0,
            1, 1,
            0, 1,
            1, 0, // BACK
            1, 1,
            0, 1,
            0, 0,
            0,1, // TOP
            0,0,
            1,0,
            1,1,
            0,0, // BOTTOM
            1,0,
            1,1,
            0,1,
            1,0, // RIGHT
            1,1,
            0,1,
            0,0,
            0,0, // LEFT
            1,0,
            1,1,
            0,1
    };

    float newNormals[] = {
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0
    };

    int newIndexes[] = {
            indexCount + 0, indexCount + 1, indexCount + 2, // FRONT
            indexCount + 2, indexCount + 3, indexCount + 0,
            indexCount + 0+4, indexCount + 1+4, indexCount + 2+4, // BACK
            indexCount + 2+4, indexCount + 3+4, indexCount + 0+4,
            indexCount + 0+8, indexCount + 1+8, indexCount + 2+8, // TOP
            indexCount + 2+8, indexCount + 3+8, indexCount + 0+8,
            indexCount + 0+12, indexCount + 1+12, indexCount + 2+12, // BOTTOM
            indexCount + 2+12, indexCount + 3+12, indexCount + 0+12,
            indexCount + 0+16, indexCount + 1+16, indexCount + 2+16, // RIGHT
            indexCount + 2+16, indexCount + 3+16, indexCount + 0+16,
            indexCount + 0+20, indexCount + 1+20, indexCount + 2+20, // LEFT
            indexCount + 2+20, indexCount + 3+20, indexCount + 0+20,
    }; // Foreach face, the indexCount is increment 4 unities

    indexCount += 24;

    float red_vec = std::get<0>(colorsVec[indColors]);
    float green_vec = std::get<1>(colorsVec[indColors]);
    float blue_vec = std::get<2>(colorsVec[indColors]);

    float newNormalColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector
    for(int i=0; i<4*6*3; i+=3){
        newNormalColors[i] = red_vec;
        newNormalColors[i+1] = green_vec;
        newNormalColors[i+2] = blue_vec;
    } // All vertices with the same color

    cubesPositions.push_back(x);
    cubesPositions.push_back(y);
    cubesPositions.push_back(z);
    cubesColors.push_back(red_vec);
    cubesColors.push_back(green_vec);
    cubesColors.push_back(blue_vec);

    float newColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector

    for(int i=0; i<4*6*3; i+=12){
        for(int j=0; j<4; j++){
            newColors[i+3*j] = red / 255.0f;
            newColors[i+3*j+1] = green / 255.0f;
            newColors[i+3*j+2] = blue / 255.0f;
        }

        int nextX = x, nextY = y, nextZ = z;

        switch(i/12){
            case 0: // FRONT
                nextZ = z+1;
                break;
            case 1: // BACK
                nextZ = z-1;
                break;
            case 2: // TOP
                nextY = y+1;
                break;
            case 3: // BOTTOM
                nextY = y-1;
                break;
            case 4: // RIGHT
                nextX = x+1;
                break;
            case 5: // LEFT
                nextX = x-1;
                break;
        }

        nextPosX[red][green][blue] = nextX;
        nextPosY[red][green][blue] = nextY;
        nextPosZ[red][green][blue] = nextZ;

        currPosX[red][green][blue] = x;
        currPosY[red][green][blue] = y;
        currPosZ[red][green][blue] = z;

        incrementColors();
    }

    // replace the values of the vbo in the next position (pre-alocated memory)
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newVertices), newVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(int), sizeof(newIndexes), newIndexes);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newColors), newColors);

    //glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    //glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newNormalColors), newNormalColors);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newNormals), newNormals);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), sizeof(newTextCoords), newTextCoords);

    vertexCount += 6 * 4;
    numIndex += 36;

    // Save the index of the cube (mapped to the position of it)
    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;

    cubeIndex++;
}

/**
 * Removes a cube from the scene
 * Basically sets all of the values in the vbo to 0
 * @param index The index of the cube
 */
void Creator::removeCube(unsigned int index) {
    GLfloat newVertices[] = {0,0,0,0,0,0,0,0,0,0,0,0, // Front
                             0,0,0,0,0,0,0,0,0,0,0,0, // Back
                             0,0,0,0,0,0,0,0,0,0,0,0, // Top
                             0,0,0,0,0,0,0,0,0,0,0,0, // Bottom
                             0,0,0,0,0,0,0,0,0,0,0,0, // Right
                             0,0,0,0,0,0,0,0,0,0,0,0}; // Left

    unsigned int newIndexes[] = {0,0,0,0,0,0, // Front
                                 0,0,0,0,0,0, // Back
                                 0,0,0,0,0,0, // Top
                                 0,0,0,0,0,0, // Bottom
                                 0,0,0,0,0,0, // Right
                                 0,0,0,0,0,0}; // Left

    float newNormalColors[] = {0,0,0,0,0,0,0,0,0,0,0,0, // Front
                               0,0,0,0,0,0,0,0,0,0,0,0, // Back
                               0,0,0,0,0,0,0,0,0,0,0,0, // Top
                               0,0,0,0,0,0,0,0,0,0,0,0, // Bottom
                               0,0,0,0,0,0,0,0,0,0,0,0, // Right
                               0,0,0,0,0,0,0,0,0,0,0,0}; // Left

    float newColors[] = {0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0};

    float newNormals[] = {0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0};

    float newTextures[] = {0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0};

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newVertices), newVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * 36 * sizeof(int), sizeof(newIndexes), newIndexes);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newColors), newColors);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newNormals), newNormals);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), sizeof(newTextures), newTextures);

    //glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    //glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newNormalColors), newNormalColors);
}

/**
 * render function
 * @param height height of the window
 * @param width width of the window
 */
void Creator::render(int height, int width){
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0, 0, 0, 0.0f); // If changes here, must change in the picking section
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    globalCamera->placeCamera();

    l.doAction(0); // Place light in the scene

    // Give semantic

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    //glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // Draw each segment of the arrays with the correct texture
    int index=0;
    int aux;
    for(std::tuple<GLuint, int> tuple: this->blockTextures){

        GLuint ind = std::get<0>(tuple);
        int numberInd = std::get<1>(tuple);
        glBindTexture(GL_TEXTURE_2D, this->texIds[ind]);
        glDrawElements(GL_TRIANGLES, numberInd, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(index));
        index+=numberInd*4;
        aux = ind;
    }

    renderText(height, width);

    // End of frame
    glutSwapBuffers();
}

/**
 * Used to pick the color of a given pixel in the screen
 * Each different face is colored (internally) with a different color
 * to be distinguished
 * @param x x position of the mouse
 * @param y y position of the mouse
 * @return the color of the pixel
 */
unsigned char* Creator::picking(int x, int y) {
    unsigned char* res = (unsigned char*)malloc(4);
    GLint viewport[4];

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    globalCamera->placeCamera();
    glDepthFunc(GL_LEQUAL);
    // Draw

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Must use the color buffer and not the texture one
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, NULL);

    glDepthFunc(GL_LESS);

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

    //glutSwapBuffers();
    //sleep(10);

    // Activate everything
    glEnableClientState(GL_TEXTURE_2D);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    return res;
}

/**
 * Deals with mouse clicks
 * Left click -> tries to remove block selected
 * Right click -> tries to place block in the correct direction
 * @param button button press
 * @param state state of button
 * @param xx x position of mouse
 * @param yy y position of mouse
 */
void Creator::processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN) {
        if (button == GLUT_RIGHT_BUTTON) { // Place block
            unsigned char *result = picking(xx, yy);
            if (result[0] != 0 || result[1] != 0 || result[2] != 0) { // If color picked is not black (background)
                printf("Picked Color %u %u %u\n", result[0], result[1], result[2]);
                printf("Bloco deve ser colocado em %f %f %f\n", nextPosX[result[0]][result[1]][result[2]],
                        nextPosY[result[0]][result[1]][result[2]],
                        nextPosZ[result[0]][result[1]][result[2]]);
                float blockX = nextPosX[result[0]][result[1]][result[2]];
                float blockY = nextPosY[result[0]][result[1]][result[2]];
                float blockZ = nextPosZ[result[0]][result[1]][result[2]];
                Point cam = this->globalCamera->getPosition();
                Point vect = Point(cam.getX()-blockX, cam.getY()-blockY, cam.getZ()-blockZ);
                if (vect.getSize() <= 8){ // Reach condition
                    addCube(nextPosX[result[0]][result[1]][result[2]],
                            nextPosY[result[0]][result[1]][result[2]],
                            nextPosZ[result[0]][result[1]][result[2]]);
                }

            } else
                printf("Nothing selected\n");
        }
        else if (button == GLUT_LEFT_BUTTON){
            unsigned char *result = picking(xx, yy);
            if (result[0] != 0 || result[1] != 0 || result[2] != 0) { // If color picked is not black (background)
                printf("Picked Color %u %u %u\n", result[0], result[1], result[2]);
                printf("Bloco deve ser removido em %f %f %f\n", currPosX[result[0]][result[1]][result[2]],
                       currPosY[result[0]][result[1]][result[2]],
                       currPosZ[result[0]][result[1]][result[2]]);
                int index = mapPointIndex[std::make_tuple(
                        currPosX[result[0]][result[1]][result[2]],
                        currPosY[result[0]][result[1]][result[2]],
                        currPosZ[result[0]][result[1]][result[2]])];
                float blockX = currPosX[result[0]][result[1]][result[2]];
                float blockY = currPosY[result[0]][result[1]][result[2]];
                float blockZ = currPosZ[result[0]][result[1]][result[2]];
                Point cam = this->globalCamera->getPosition();
                Point vect = Point(cam.getX()-blockX, cam.getY()-blockY, cam.getZ()-blockZ);
                cout << index << endl;
                if (vect.getSize() <= 8){ // Reach position
                    removeCube(index);
                    // Mark this block unused
                    cubesPositions[index*3] = 1000000;
                    cubesPositions[index*3+1] = 1000000;
                    cubesPositions[index*3+2] = 1000000;
                    cubesColors[index*3] = -1;
                    cubesColors[index*3+1] = -1;
                    cubesColors[index*3+1] = -1;
                }

            } else
                printf("Nothing selected\n");
        }
    }
}

/**
 * Used to load a texture, giving it a texture id
 * @param path the path of the texture file
 */
void Creator::loadTexture(string path) {
    unsigned int t,tw,th;
    unsigned char *texData;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)path.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1,&texIds[textInd]);

    glBindTexture(GL_TEXTURE_2D,texIds[textInd]);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

    // Possibility to change the filters here
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    glBindTexture(GL_TEXTURE_2D, 0);
    textInd++;
}

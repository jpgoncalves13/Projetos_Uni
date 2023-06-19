#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../libraries/tinyxml2.h"
#include "camera.hpp"
#include "group.hpp"
#include "creator.h"
#include "Lights/Light.h"
#include "Lights/LightPoint.h"
#include "Lights/LightDirectional.h"
#include "Lights/LightSpot.h"
#include "BoundingVolumes/plane.hpp"

#define INDEX(row, col) ((col) * 4 + (row) - 5)

using namespace std;

int width, height;
Camera* camera;
int lastxx, lastyy;
Group* globalGroup; // Outer collection of transformations, models and subgroups

// For FPS count
int timebase;
float frames;

bool axis = true; // Is axis shown
bool cameraInfo = false;
bool fixedMode = false;
string fixedLabel = "";
int polygonMode = GL_FILL;

// For each label, store the center and the radius
map<string, tuple<Point, float>> teleports;
vector<string> keys; // To make mapping from number to label easier

bool vboActive = true;

// TEXTURES

bool mipmap = false;
bool change = false;

// Minecraft
bool isMinecraftActive = false;
Creator* minecraftCreator;

// Lights
vector<Light*> lightSources;

/**
 * Callback called when the window is resized
 * @param w width of the window
 * @param h height of the window
 */
void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	camera->setRatio((float) w * 1.0f / (float) h);

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(camera->getFov() ,camera->getRatio(), camera->getNear() , camera->getFar());
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

/**
 * Function to render text
 */
void renderText() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // set projection so that coordinates match window pixels
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glLoadIdentity();
    glRasterPos2d(0, 0); // text position in pixels

    if (cameraInfo)
        for (const unsigned char *c = camera->toString(); *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
        }

    if (camera->getMode() == 1) {
        glRasterPos2d(width/2 - 1, height/2 - 1); // text position in pixels
        for (const char *c = "+"; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}


/**
 * Function called for fps calculation
*/
void showFPS(int nt) {
    frames++;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int fps;

    if (time - timebase > 1000) {
        fps = frames * 1000.0 / (time - timebase);
        timebase = time;
        frames = 0;
        char s[10000];
        sprintf(s, "FPS: %d  Triangles: %d", fps, nt);
            glutSetWindowTitle(s);
    }
}

Plane* getPlanes(float *matrix) {
    Plane *planes = new Plane[6];

    // Left plane
    float a = matrix[INDEX(4, 1)] + matrix[INDEX(1, 1)];
    float b = matrix[INDEX(4, 2)] + matrix[INDEX(1, 2)];
    float c = matrix[INDEX(4, 3)] + matrix[INDEX(1, 3)];
    float d = matrix[INDEX(4, 4)] + matrix[INDEX(1, 4)];
    float l = sqrt(a * a + b * b + c * c);
    planes[0].setCoefficients(a/l, b/l, c/l, d/l);

    // Right plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(1, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(1, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(1, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(1, 4)];
    l = sqrt(a * a + b * b + c * c);
    planes[1].setCoefficients(a/l, b/l, c/l, d/l);

    // Top plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(2, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(2, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(2, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(2, 4)];
    l = sqrt(a * a + b * b + c * c);
    planes[2].setCoefficients(a/l, b/l, c/l, d/l);

    // Bottom plane
    a = matrix[INDEX(4, 1)] + matrix[INDEX(2, 1)];
    b = matrix[INDEX(4, 2)] + matrix[INDEX(2, 2)];
    c = matrix[INDEX(4, 3)] + matrix[INDEX(2, 3)];
    d = matrix[INDEX(4, 4)] + matrix[INDEX(2, 4)];
    l = sqrt(a * a + b * b + c * c);
    planes[3].setCoefficients(a/l, b/l, c/l, d/l);

    // Far plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(3, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(3, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(3, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(3, 4)];
    l = sqrt(a * a + b * b + c * c);
    planes[4].setCoefficients(a/l, b/l, c/l, d/l);

    // Near plane
    a = matrix[INDEX(4, 1)] + matrix[INDEX(3, 1)];
    b = matrix[INDEX(4, 2)] + matrix[INDEX(3, 2)];
    c = matrix[INDEX(4, 3)] + matrix[INDEX(3, 3)];
    d = matrix[INDEX(4, 4)] + matrix[INDEX(3, 4)];
    l = sqrt(a * a + b * b + c * c);
    planes[5].setCoefficients(a/l, b/l, c/l, d/l);

    return planes;
}



/**
 * Function called to show the axis
*/
void drawAxis() {
    if (axis) {
        glDisable(GL_LIGHTING);
        // put axis drawing in here
        glBegin(GL_LINES);
        // X-axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
    }
}

/**
 * Function to display the scene elements:
 * Calls the drawGroup function from the group class
 */
void renderScene() {
    // Set the polygon mode 
    glPolygonMode(GL_FRONT, polygonMode);

    if (!isMinecraftActive) {
        // clear buffers
        glClearColor(0, 0, 0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the camera
        glLoadIdentity();
        if (fixedMode == true && fixedLabel.compare("") != 0) {
            camera->setLookAtPosition(get<0>(teleports[fixedLabel]));
            camera->setCameraRadius(get<1>(teleports[fixedLabel]));
            camera->spherical2Cartesian();
            camera->setMode(0);
        }
        camera->placeCamera();

        // Place lights
        for (int i = 0; i < lightSources.size(); i++) {
            lightSources[i]->doAction(GL_LIGHT0 + i);
        }

        float position[16] = { 1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1,
        };

        float M[16],P[16];
        glGetFloatv(GL_MODELVIEW_MATRIX,M);
        glGetFloatv(GL_PROJECTION_MATRIX,P);
        glPushMatrix();
        glLoadMatrixf(P);
        glMultMatrixf(M);
        float A[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, A);
        glPopMatrix();

        Plane *planes = getPlanes(A);

        int nt = globalGroup->drawGroup(vboActive, planes, position, &teleports, mipmap, change);

        delete[] planes;

        if (change == true) change = false;

        // Auxiliary functions
        renderText();
        drawAxis();
        showFPS(nt);
         
        // End of frame
        glutSwapBuffers();
    }
    else{
        minecraftCreator->render(height, width);
        showFPS(0);
    }
}

/**
 * Given the id of the menu, controls the changes to make
 * @param id id of the menu
 */
void menu(int id)
{
    switch(id)
    {
        case 1:
            if (mipmap == true){
                mipmap = false;
                change = true;
            }
            break;
        case 2:
            if (mipmap == false){
                mipmap = true;
                change = true;
            }
        case 3:
            camera->changeMode(0); // Explorer
            break;
        case 4:
            camera->changeMode(1); // FPS
            break;
        case 5:
            camera->changeMode(2); // Mouse motion
            break;
        case 6:
            fixedMode = !fixedMode;
            fixedLabel = "";
            break;
        case 7:
            break;
        case 8:
            vboActive = !vboActive;
            break;
        case 9:
            cameraInfo = !cameraInfo;
            break;
        case 10:
            axis = !axis;
            break;
        case 11:
            polygonMode = GL_FILL;
            break;
        case 12:
            polygonMode = GL_LINE;
            break;
        case 13:
            polygonMode = GL_POINT;
            break;
        case 14:
            camera->setLookAtPosition(Point(0,0,0));
            camera->setCameraRadius(5000);
            camera->setAlpha(0.785); // ~ 45º
            camera->setBeta(0.785); // ~ 45º
            camera->spherical2Cartesian();
            camera->setMode(0);
            break;
        default:
            fixedLabel = keys[id-15];
            camera->setLookAtPosition(get<0>(teleports[keys[id-15]]));
            camera->setCameraRadius(get<1>(teleports[keys[id-15]]));
            camera->setAlpha(0.785); // ~ 45º
            camera->setBeta(0.785); // ~ 45º
            camera->spherical2Cartesian();
            camera->setMode(0);
            break;
    }
    glutPostRedisplay();
}

/**
 * Creates a menu with options of:
 * - toggling axis
 * - travelling to planet locations
 */
void createMenu(void){
    int submenu2, submenu3, submenu4, submenu5;

    submenu2 = glutCreateMenu(menu);
    glutAddMenuEntry("GL_FILL", 11);
    glutAddMenuEntry("GL_LINE", 12);
    glutAddMenuEntry("GL_POINT", 13);

    submenu3 = glutCreateMenu(menu);
    glutAddMenuEntry("", 1);
    glutAddMenuEntry("Origin", 14);
    int i=0;
    for (string label:keys) {
        const char* l = label.c_str();
        glutAddMenuEntry(l, 15+i);
        i++;
    }

    submenu4 = glutCreateMenu(menu);
    glutAddMenuEntry("Explorer mode", 3);
    glutAddMenuEntry("FPS mode", 4);
    glutAddMenuEntry("Mouse mode", 5);

    submenu5 = glutCreateMenu(menu);
    glutAddMenuEntry("No Mipmap", 1);
    glutAddMenuEntry("Mipmap", 2);

    glutCreateMenu(menu);
    glutAddSubMenu("Travel To", submenu3);
    glutAddSubMenu("Change polygon mode", submenu2);
    glutAddSubMenu("Camera mode", submenu4);

    glutAddMenuEntry("Change Teleport Mode", 6);
    glutAddMenuEntry("Add axes", 10);
    glutAddMenuEntry("Show camera info", 9);
    glutAddMenuEntry("Toggle vbo mode", 8);
    glutAddSubMenu("Textures", submenu5);

    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

/**
 * Processing callback to handle mouse events
 * @param button
 * @param state
 * @param xx
 * @param yy
 */
void processMouseButtons(int button, int state, int xx, int yy) {
    if (isMinecraftActive == true){
        minecraftCreator->processMouseButtons(button, state, xx, yy);
    } else {
        camera->updateMouseAngles(button, state, xx, yy);
    }
}

/**
 * To be called when the mouse moves
 * @param xx horizontal position of the mouse
 * @param yy vertical position of the mouse
 */
void processMouseMotion(int xx, int yy) {
    camera->processMouseMotion(xx, yy, lastxx, lastyy);
    lastxx = xx;
    lastyy = yy;
}

/**
 * function to process keyboard events (only for non-special keys)
 * @param key key pressed
 * @param x x coordinate of mouse in the screen
 * @param y y coordinate of mouse in the screen
 */
void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W'){
        camera->moveForwards();
    }
    else if (key == 's' || key == 'S'){
        camera->moveBackwards();
    }
    else if (key == 'a' || key == 'A'){
        camera->moveLeft();
    }
    else if (key == 'd' || key == 'D'){
        camera->moveRight();
    }
    else if (key == '1'){
        camera->decrementIncrement();
    }
    else if (key == '2'){
        camera->incrementIncrement();
    }
    else if (key == 'm'){
        if (isMinecraftActive == true) {
            isMinecraftActive = false;
            glDisableClientState(GL_COLOR_ARRAY);
            minecraftCreator->~Creator();
        }
        else {
            isMinecraftActive = true;
            //Config minecraft state
            for (int i = 0; i < lightSources.size(); i++) {
                glDisable(GL_LIGHT0 + i);
            }
            // Ativar luz 0
            float dark[4] = {0.2, 0.2, 0.2, 1.0};
            float white[4] = {1.0, 1.0, 1.0, 1.0};
            float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
            glLightfv(GL_LIGHT0, GL_SPECULAR, white);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
            polygonMode = GL_FILL;
            camera->changeMode(1);
            camera->setPosition(Point(0,5,0));
            minecraftCreator = new Creator(camera);
        }
    }
    else if (key == 'v'){
        minecraftCreator->changeBlockTexture();
    }
    glutPostRedisplay();
}

/**
 * function to process keyboard events (for special keys)
 * @param key key pressed
 * @param x x coordinate of mouse in the screen
 * @param y y coordinate of mouse in the screen
 */
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            camera->incrementAlfa(); break; // horizontal rotation

        case GLUT_KEY_LEFT:
            camera->decrementAlfa(); break; // horizontal rotation

        case GLUT_KEY_UP:
            camera->incrementBeta(); break; // vertical rotation

        case GLUT_KEY_DOWN:
            camera->decrementBeta(); break; // horizontal rotation

        case GLUT_KEY_PAGE_DOWN:
            camera->decrementRadius(); break;

        case GLUT_KEY_PAGE_UP:
            camera->incrementRadius(); break;

        case GLUT_KEY_HOME:
            camera->moveUp(); break;

        case GLUT_KEY_END:
            camera->moveDown(); break;
    }

    glutPostRedisplay();
}

/**
 * Parsing of the XML with the informations for the camera settings and groups
 * @param filePath the PATH of the XML
 * @return an error code (0 - ok, > 0 - something has gone wrong)
 */
int readXML(char* filePath, vector<string>* keys){
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filePath);
    tinyxml2::XMLNode* world = doc.FirstChildElement("world");

    float fov = 0, farV = 0, nearV = 0;
    Point cameraPosition, cameraLookAt, cameraUpVector;
  
    if (!error && world) {
        /* window */
        tinyxml2::XMLElement* windowElem = world->FirstChildElement("window");
        if (windowElem) {
            width =  stoi(windowElem->Attribute("width"));
            height = stoi(windowElem->Attribute("height"));
        }
        
        /* camera */
        tinyxml2::XMLElement* cameraElem = world->FirstChildElement("camera");
        if (cameraElem) {
            
            /* camera position */
            tinyxml2::XMLElement* position = cameraElem->FirstChildElement("position");
            if (position) {
                cameraPosition.setX(stof(position->Attribute("x")));
                cameraPosition.setY(stof(position->Attribute("y")));
                cameraPosition.setZ(stof(position->Attribute("z")));
            }

            /* camera lookAt */
            tinyxml2::XMLElement* lookAt = cameraElem->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt.setX(stof(lookAt->Attribute("x")));
                cameraLookAt.setY(stof(lookAt->Attribute("y")));
                cameraLookAt.setZ(stof(lookAt->Attribute("z")));
            }

            /* camera up */
            tinyxml2::XMLElement* up = cameraElem->FirstChildElement("up");
            if (up) {
                cameraUpVector.setX(stof(up->Attribute("x")));
                cameraUpVector.setY(stof(up->Attribute("y")));
                cameraUpVector.setZ(stof(up->Attribute("z")));
            }
            
            /* camera projection */
            tinyxml2::XMLElement* projection = cameraElem->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                nearV = stof(projection->Attribute("near"));
                farV = stof(projection->Attribute("far"));
            }
          
            camera = new Camera(cameraPosition, cameraLookAt, cameraUpVector, fov, nearV, farV);
        }
        // Lights
        tinyxml2::XMLElement* lightsElem = world->FirstChildElement("lights");
        if (lightsElem) {
            for (XMLElement* light = lightsElem->FirstChildElement(); light != NULL; light = light->NextSiblingElement()) {
                string type = light->Attribute("type");

                if (type.compare("point") == 0){
                    const char* posX = light->Attribute("posx") ? light->Attribute("posx") : light->Attribute("posX");
                    const char* posY = light->Attribute("posy") ? light->Attribute("posy") : light->Attribute("posY");
                    const char* posZ = light->Attribute("posz") ? light->Attribute("posz") : light->Attribute("posZ");
                    if (posX && posY && posZ) {
                        LightPoint* lp = new LightPoint(stof(posX), stof(posY), stof(posZ));
                        lightSources.push_back(lp);
                    }
                }
                else if (type.compare("directional") == 0){
                    const char* dirX = light->Attribute("dirx") ? light->Attribute("dirx") : light->Attribute("dirX");
                    const char* dirY = light->Attribute("diry") ? light->Attribute("diry") : light->Attribute("dirY");
                    const char* dirZ = light->Attribute("dirz") ? light->Attribute("dirz") : light->Attribute("dirZ");
                    if (dirX && dirY && dirZ) {
                        LightDirectional* ld = new LightDirectional (stof(dirX), stof(dirY), stof(dirZ));
                        lightSources.push_back(ld);
                    }
                }
                else if (type.compare("spotlight") == 0 || type.compare("spot") == 0) {
                    const char* posX = light->Attribute("posx") ? light->Attribute("posx") : light->Attribute("posX");
                    const char* posY = light->Attribute("posy") ? light->Attribute("posy") : light->Attribute("posY");
                    const char* posZ = light->Attribute("posz") ? light->Attribute("posz") : light->Attribute("posZ");
                    const char* dirX = light->Attribute("dirx") ? light->Attribute("dirx") : light->Attribute("dirX");
                    const char* dirY = light->Attribute("diry") ? light->Attribute("diry") : light->Attribute("dirY");
                    const char* dirZ = light->Attribute("dirz") ? light->Attribute("dirz") : light->Attribute("dirZ");
                    const char* cutoff = light->Attribute("cutoff");
                    if (posX && posY && posZ && dirX && dirY && dirZ && cutoff) {
                        LightSpot* ls = new LightSpot(stof(posX), stof(posY), stof(posZ), stof(dirX), stof(dirY), stof(dirZ), stof(cutoff));
                        lightSources.push_back(ls);
                    }
                }
                else {
                    cout << "Unknown light type: " << type << endl;
                }
            }
        }
        globalGroup = new Group();
        // Read the group
        globalGroup->readXML(world->FirstChildElement("group"), keys);
    }

    return (int) error;
}

// Main function
int main(int argc, char **argv) {
    if (argc == 2) {

        // init GLUT and the window
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);
        glutCreateWindow("ProjetoCG");

        timebase = glutGet(GLUT_ELAPSED_TIME);

        // Required callback registry
        glutIdleFunc(renderScene);
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);


        // put here the registration of the keyboard callbacks
        glutSpecialFunc(processSpecialKeys);
        glutKeyboardFunc(keyboard_events);
        glutPassiveMotionFunc(processMouseMotion);
        glutMouseFunc(processMouseButtons);
        glutMotionFunc(processMouseMotion);

        // Required for VBOs
        glewInit();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        int error = readXML(argv[1], &keys);

        if (!error) {
            createMenu();

            // 	OpenGL settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glEnable(GL_RESCALE_NORMAL);
            glEnable(GL_TEXTURE_2D);

            glPolygonMode(GL_FRONT, polygonMode);

            // Colors
            float dark[4] = {0.2, 0.2, 0.2, 1.0};
            float white[4] = {1.0, 1.0, 1.0, 1.0};
            float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                
            // Activate lightning
            glEnable(GL_LIGHTING);
            for (int i = 0; i < lightSources.size(); i++) {
                glEnable(GL_LIGHT0 + i);
                glLightfv(GL_LIGHT0 + i, GL_AMBIENT, dark);
                glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
                glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
            }
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);

            
            // enter GLUT's main cycle
            glutMainLoop();

            globalGroup->freeGroup();
            delete globalGroup;
        } else {
            cout << "Error reading the xml." << endl;
        }
    }
    else {
        cout << "Número incorreto de argumentos." << endl;
    }
	
	return 1;
}

//
// Created by mike on 10-05-2023.
//

#include "LightDirectional.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


LightDirectional::LightDirectional(float dirX, float dirY, float dirZ) : dirX(dirX), dirY(dirY), dirZ(dirZ) {}

float LightDirectional::getDirX() {
    return dirX;
}

float LightDirectional::getDirY() {
    return dirY;
}

float LightDirectional::getDirZ() {
    return dirZ;
}

void LightDirectional::doAction(int light){ 
    float pos[4] = {this->getDirX(), this->getDirY(), this->getDirZ(), 0};
    glLightfv(light, GL_POSITION, pos);
}
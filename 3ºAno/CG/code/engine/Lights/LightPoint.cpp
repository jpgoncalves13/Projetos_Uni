//
// Created by mike on 10-05-2023.
//

#include "LightPoint.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

LightPoint::LightPoint(float posX, float posY, float posZ) : posX(posX), posY(posY), posZ(posZ) {}

float LightPoint::getPosX() {
    return posX;
}

float LightPoint::getPosY() {
    return posY;
}

float LightPoint::getPosZ() {
    return posZ;
}

void LightPoint::doAction(int light) {
    float pos[4] = {this->getPosX(), this->getPosY(), this->getPosZ(), 1};
    glLightfv(light, GL_POSITION, pos);
}
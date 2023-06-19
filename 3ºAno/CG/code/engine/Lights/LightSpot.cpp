//
// Created by mike on 10-05-2023.
//

#include "LightSpot.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

LightSpot::LightSpot(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff) : posX(posX),
                                                                                                             posY(posY),
                                                                                                             posZ(posZ),
                                                                                                             dirX(dirX),
                                                                                                             dirY(dirY),
                                                                                                             dirZ(dirZ),
                                                                                                             cutoff(cutoff) {}

float LightSpot::getPosX() {
    return posX;
}

float LightSpot::getPosY() {
    return posY;
}

float LightSpot::getPosZ() {
    return posZ;
}

float LightSpot::getDirX() {
    return dirX;
}

float LightSpot::getDirY() {
    return dirY;
}

float LightSpot::getDirZ() {
    return dirZ;
}

float LightSpot::getCutoff() {
    return cutoff;
}

void LightSpot::doAction(int light) {
    float pos[4] = {this->getPosX(), this->getPosY(), this->getPosZ(), 1};
    float dir[3] = {this->getDirX(), this->getDirY(), this->getDirZ()};
    float cutoff = this->getCutoff();
    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_SPOT_DIRECTION, dir);
    glLightfv(light, GL_SPOT_CUTOFF, &cutoff);
}
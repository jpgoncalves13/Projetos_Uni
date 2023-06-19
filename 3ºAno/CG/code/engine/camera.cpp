#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "camera.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "stdio.h"

Camera::Camera(Point position,Point lookAtPosition,Point upVector, float fov, float near,float far) {
    this->position = position;
    this->lookAtPosition = lookAtPosition;
    this->upVector = upVector;
    this->fov = fov;
    this->nearV = near;
    this->farV = far;
    this->mode = 0;
    this->d.setPoint(0,0,0);
    this->calculateSpherical(); //  Enter explorer mode
    this->spherical2Cartesian();
}

/**
 * Calculate alfa, beta and radius, necessary for the explorer camera mode
 */
void Camera::calculateSpherical(){
    this->cameraRadius = sqrt(pow(this->position.getX()-this->lookAtPosition.getX(), 2) +
                              pow(this->position.getY()-this->lookAtPosition.getY(), 2) +
                              pow(this->position.getZ()-this->lookAtPosition.getZ(), 2));
    this->increment = 0.1;
    calculateBeta();
    calculateAlfa();
}

void Camera::setMode(int mode) {
    this->mode = mode;
}


void Camera::setAlpha(float angle) {
    this->alfa = angle;
}


void Camera::setBeta(float angle) {
    this->beta = angle;
}


/**
 * Given the cameraRadius, alfa and beta, calculate the camera position
 */
void Camera::spherical2Cartesian() {
    this->position.setX(this->cameraRadius * cos(this->beta) * sin(this->alfa) + this->lookAtPosition.getX());
    this->position.setY(this->cameraRadius * sin(this->beta) + this->lookAtPosition.getY());
    this->position.setZ(this->cameraRadius * cos(this->beta) * cos(this->alfa) + this->lookAtPosition.getZ());
}

/**
 * Increase the horizontal angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementAlfa(){
    if (this->mode == 1){
        this->alfa -= this->increment;
        this->calculateDirection();
    }
    else if (this->mode == 0){
        this->alfa += this->increment;
        this->spherical2Cartesian();
    }
}

/**
 * Decrease the horizontal angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementAlfa(){
    if (this->mode == 1){
        this->alfa += this->increment;
        this->calculateDirection();
    }
    else if (this->mode == 0){
        this->alfa -= this->increment;
        this->spherical2Cartesian();
    }
}

/**
 * Increase the vertical angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementBeta(){
    if (this->mode == 1 || this->mode == 0) {
        this->beta += this->increment;
        if (beta > 1.5f)
            beta = 1.5f;
    }
    if (this->mode == 1){
        this->calculateDirection();
    }
    else if (this->mode == 0) {
        this->spherical2Cartesian();
    }
}

/**
 * Decrease the vertical angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementBeta(){
    if (this->mode == 1 || this->mode == 0) {
        this->beta -= this->increment;
        if (beta < -1.5f)
            beta = -1.5f;
    }
    if (this->mode == 1){
        this->calculateDirection();
    }
    else if (this->mode == 0){
        this->spherical2Cartesian();
    }
}

/**
 * Increase the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementRadius(){
    if (this->mode == 0) {
        this->cameraRadius += this->increment;
        this->spherical2Cartesian();
    }
}

/**
 * Decrease the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementRadius(){
    if (this->mode == 0) {
        this->cameraRadius -= this->increment;
        if (cameraRadius < 0.1f)
            cameraRadius = 0.1f;
        this->spherical2Cartesian();
    }
}

/**
 * Increase increment of the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementIncrement() {
    if (this->mode == 1){
        this->step += 1;
    }
    else if (this->mode == 0){
        this->increment += 0.01f;
    }

}

/**
 * Decrease increment of the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementIncrement() {
    if (this->mode == 1){
        this->step -= 1;
        if (this->step < 1)
            this->step = 1;
    }
    else if (this->mode == 0) {
        this->increment -= 0.01;
        if (this->increment < 0.01f){
            this->increment = 0.01f;
        }
    }

}

/**
 * Calculates the vector of the direction the camera is looking
 * given alfa and beta
 */
void Camera::calculateDirection() {
    this->d.setX(cos(beta) * sin(alfa));
    this->d.setY(sin(beta));
    this->d.setZ(cos(beta) * cos(alfa));
}

/**
 * In FPS mode, goes to the direction it is looking
 */
void Camera::moveForwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() + this->step*this->d.getX());
        this->position.setY(this->position.getY() + this->step*this->d.getY());
        this->position.setZ(this->position.getZ() + this->step*this->d.getZ());
    }
}

/**
 * In FPS, goes to the opposite direction it is looking
 */
void Camera::moveBackwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() - this->step*this->d.getX());
        this->position.setY(this->position.getY() - this->step*this->d.getY());
        this->position.setZ(this->position.getZ() - this->step*this->d.getZ());
    }
}

void Camera::moveLeft() {
    if (mode == 1){ // Modo FPS
        Point perp = Point::crossProduct(d, upVector);
        perp.normalize();
        this->position.setX(this->position.getX() - this->step*perp.getX());
        this->position.setY(this->position.getY() - this->step*perp.getY());
        this->position.setZ(this->position.getZ() - this->step*perp.getZ());
    }
}

void Camera::moveRight() {
    if (mode == 1){ // Modo FPS
        Point perp = Point::crossProduct(d, upVector);
        perp.normalize();
        this->position.setX(this->position.getX() + this->step*perp.getX());
        this->position.setY(this->position.getY() + this->step*perp.getY());
        this->position.setZ(this->position.getZ() + this->step*perp.getZ());
    }
}

void Camera::moveUp() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() + this->step*upVector.getX());
        this->position.setY(this->position.getY() + this->step*upVector.getY());
        this->position.setZ(this->position.getZ() + this->step*upVector.getZ());
    }
}

void Camera::moveDown() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() - this->step*upVector.getX());
        this->position.setY(this->position.getY() - this->step*upVector.getY());
        this->position.setZ(this->position.getZ() - this->step*upVector.getZ());
    }
}

/**
 * Changes the camera mode
 */
void Camera::changeMode(int desiredMode){
    if (desiredMode == 0) {
        // Explorer
        this->mode = 0;
        this->d = Point(0,0,0);
        this->calculateSpherical();
        this->spherical2Cartesian();
    }
    else if (desiredMode == 1) {
        // FPS
        this->mode = 1;
        this->d = Point((this->lookAtPosition.getX() - this->position.getX()),
                        (this->lookAtPosition.getY() - this->position.getY()),
                        (this->lookAtPosition.getZ() - this->position.getZ()));
        this->d.normalize();
        this->calculateAlfa();
        this->calculateBeta();
        this->step = 1;
    }
    else if (desiredMode == 2) {
        // Mouse mode
        this->mode = 2;
        this->d = Point(0,0,0);
        this->calculateSpherical();
        this->spherical2Cartesian();
    }
}

Point Camera::getPosition() {
    return position;
}

Point Camera::getLookAtPosition() {
    return lookAtPosition;
}

Point Camera::getUpVector() {
    return upVector;
}

float Camera::getFov() {
    return fov;
}

float Camera::getNear() {
    return nearV;
}

float Camera::getFar() {
    return farV;
}

float Camera::getRatio() {
    return this->ratio;
}
    
void Camera::setRatio(float ratio) {
    this->ratio = ratio;
}

void Camera::updateMouseAngles(int button, int state, int xx, int yy){
    if (mode == 2){
        if (state == GLUT_DOWN)  {
            startX = xx;
            startY = yy;
            if (button == GLUT_LEFT_BUTTON)
                tracking = 1;
            else if (button == GLUT_RIGHT_BUTTON)
                tracking = 2;
            else
                tracking = 0;
        }
        else if (state == GLUT_UP) {
            if (tracking == 1) {
                alfa += (xx - startX);
                beta += (yy - startY);
            }
            else if (tracking == 2) {

                cameraRadius -= yy - startY;
                if (cameraRadius < 3)
                    cameraRadius = 3.0;
            }
            tracking = 0;
        }
    }
}

void Camera::processMouseMotion(int xx, int yy, int lastxx, int lastyy) {
    if (mode == 1){
        int deltaX = xx - lastxx;
        int deltaY = yy - lastyy;

        alfa = alfa - (float)deltaX/200.0f;
        beta = beta - (float)deltaY/200.0f;

        if (beta > M_PI/2-0.2f)
            beta = M_PI/2-0.2f;
        else if (beta < -M_PI/2+0.2f)
            beta = -M_PI/2+0.2f;
        this->calculateDirection();
    }
    else if (mode == 2){
        int deltaX, deltaY;
        int alphaAux, betaAux;
        int rAux;

        if (!tracking)
            return;

        deltaX = xx - startX;
        deltaY = yy - startY;

        if (tracking == 1) {
            alphaAux = alfa + deltaX;
            betaAux = beta + deltaY;

            if (betaAux > 85.0)
                betaAux = 85.0;
            else if (betaAux < -85.0)
                betaAux = -85.0;

            rAux = cameraRadius;
        }
        else if (tracking == 2) {
            alphaAux = alfa;
            betaAux = beta;
            rAux = cameraRadius - deltaY;
            if (rAux < 3)
                rAux = 3;
        }
        position.setX( rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0) + lookAtPosition.getX());
        position.setZ(rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0) + lookAtPosition.getY()) ;
        position.setY(rAux * sin(betaAux * 3.14 / 180.0) + lookAtPosition.getZ());
    }
}

/**
 * Places the camera in the world, using the glut gluLookAt function
 */
void Camera::placeCamera(){
    if (mode == 0 || mode == 2){
        gluLookAt(this->getPosition().getX(),this->getPosition().getY(),this->getPosition().getZ(),
                  this->getLookAtPosition().getX() ,this->getLookAtPosition().getY(),this->getLookAtPosition().getZ(),
                  this->getUpVector().getX(),this->getUpVector().getY(),this->getUpVector().getZ());
    } else if (mode == 1){
        gluLookAt(this->getPosition().getX(),this->getPosition().getY(),this->getPosition().getZ(),
                  this->getPosition().getX() + this->d.getX(),this->getPosition().getY() + this->d.getY(),this->getPosition().getZ() + this->d.getZ(),
                  this->getUpVector().getX(),this->getUpVector().getY(),this->getUpVector().getZ());
    }
}

/**
 * Returns a string with the camera details
 * @return
 */
const unsigned char *Camera::toString() {
    const unsigned char* s = (const unsigned char*)malloc(200);
    sprintf((char *) s, "MODE: %d    ALFA: %f      BETA: %f      POSITION: %s      RADIUS: %f      LOOK AT: %s", mode, alfa * 180/M_PI, beta * 180/M_PI, this->position.toString().c_str(), this->cameraRadius, this->lookAtPosition.toString().c_str());
    return s;
}

/**
 * Calculates alfa based on:
 * - the position the camera is, if in explorer mode
 */
void Camera::calculateAlfa(){
    if (d.getX() == 0 && d.getY()==0 && d.getZ()==0) {
        float vx = this->position.getX() - this->lookAtPosition.getX();
        float vz = this->position.getZ() - this->lookAtPosition.getZ();
        alfa = atan2(vx, vz);
    } else {
        float vx = this->lookAtPosition.getX() - this->position.getX();
        float vz = this->lookAtPosition.getZ() - this->position.getZ();
        alfa = atan2(vx, vz);
    }
}

/**
 * Calculates beta based on:
 * - the position the camera is, if in explorer mode
 */
void Camera::calculateBeta(){
    if(d.getX() == 0 && d.getY()==0 && d.getZ()==0) {
        float vy = this->position.getY() - this->lookAtPosition.getY();
        float vx = this->position.getX() - this->lookAtPosition.getX();
        float vz = this->position.getZ() - this->lookAtPosition.getZ();
        float dist = sqrt(pow(vx, 2) + pow(vz, 2));
        beta = atan2(vy, dist);
    } else {
        float vy = this->lookAtPosition.getY() - this->position.getY();
        float vx = this->lookAtPosition.getX() - this->position.getX();
        float vz = this->lookAtPosition.getZ() - this->position.getZ();
        float dist = sqrt(pow(vx, 2) + pow(vz, 2));
        beta = atan2(vy, dist);
    }
}

void Camera::setPosition(Point position) {
    this->position = position;
}

void Camera::setLookAtPosition(Point lookAtPosition) {
    Camera::lookAtPosition = lookAtPosition;
}

void Camera::setCameraRadius(float cameraRadius) {
    Camera::cameraRadius = cameraRadius;
    this->spherical2Cartesian();
}

float Camera::getCameraRadius() {
    return cameraRadius;
}

int Camera::getMode() {
    return this->mode;
}
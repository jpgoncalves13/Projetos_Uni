#ifndef CAMERA
#define CAMERA

#include <iostream>
#include "../shared/point.hpp"
using namespace std;

class Camera
{
private:
    Point position;
    Point lookAtPosition;
    Point upVector;
    float fov, nearV, farV, ratio;
    float increment;
    int mode; // 0 - Explorer mode, 1 - FPS mode, 2 - Mouse mode
    float alfa, beta, cameraRadius;
    float step;
    Point d;
    int tracking = 0;
    int startX, startY;

public:
    Camera();
    Camera(Point position, Point lookAtPosition, Point upVector, float fov, float nearV, float farV);
    void setPosition(Point position);
    void setMode(int mode);
    void setAlpha(float angle);
    void setBeta(float beta);
    void incrementAlfa();
    void decrementAlfa();
    void incrementBeta();
    void decrementBeta();
    float getCameraRadius();
    void incrementRadius();
    void decrementRadius();
    void incrementIncrement();
    void decrementIncrement();
    void moveForwards();
    void moveBackwards();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void changeMode(int mode); // -1 goes to the next
    void placeCamera();
    void updateMouseAngles(int button, int state, int xx, int yy);
    void processMouseMotion(int xx, int yy, int lastxx, int lastyy);
    const unsigned char* toString();
    Point getPosition();
    Point getLookAtPosition();
    Point getUpVector();
    float getFov();
    float getNear();
    float getFar();
    float getRatio();
    void setRatio(float ratio);
    void setLookAtPosition(Point lookAtPosition);
    void setCameraRadius(float cameraRadius);
    void spherical2Cartesian();
    void calculateDirection();
    void calculateAlfa();
    void calculateBeta();
    void calculateSpherical();
    int getMode();
};

#endif

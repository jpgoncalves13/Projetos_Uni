#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "../../shared/point.hpp"

class Plane {
public:
    Plane();
    Plane(float a, float b, float c, float d);
    void setCoefficients(float a, float b, float c, float d);
    bool inRightSide(Point p);
    float distanceToPoint(Point p);

private:
    float a;
    float b;
    float c;
    float d;
};


#endif 

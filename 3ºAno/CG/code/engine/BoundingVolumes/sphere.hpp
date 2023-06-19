#ifndef ENGINE_SPHERE_H
#define ENGINE_SPHERE_H

#include "volume.hpp"
#include "../../shared/point.hpp"
#include "../camera.hpp"
#include "plane.hpp"

class Sphere : public Volume {
public:
    Sphere(float radius, Point center);
    bool test(Plane *planes, float* actual_matrix);
    Sphere();
    void setRadius(float radius);
    float getRadius();
    void setCenter(Point center);
    Point getCenter();

private:
    float radius;
    Point center;
};


#endif 

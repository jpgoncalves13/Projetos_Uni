#ifndef ENGINE_AABB_H
#define ENGINE_AABB_H

#include "volume.hpp"
#include "../../shared/point.hpp"
#include "plane.hpp"

class AABB : public Volume {
public:
    AABB(Point* corners);
    AABB();
    void setCorners(Point* corners);
    bool test(Plane *planes, float *actual_matrix);
    Point* getCorners();
private:
    Point corners[8];
};


#endif 

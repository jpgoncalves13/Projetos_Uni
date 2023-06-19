#ifndef ENGINE_VOLUME_H
#define ENGINE_VOLUME_H

#include "../camera.hpp"
#include "plane.hpp"

class Volume {
public:
    Volume();
    virtual bool test(Plane *planes, float* actual_matrix);
};


#endif

#ifndef ENGINE_TRANSFORMATION_HPP
#define ENGINE_TRANSFORMATION_HPP

#include "../../shared/point.hpp"

class Transformation{
private:
    float x, y, z;

public:
    virtual void applyTransformation(float *matrix);
    Transformation(float x, float y, float z);
    float getX();
    void setX(float x);
    float getY();
    void setY(float y);
    float getZ();
    void setZ(float z);
};

#endif //ENGINE_TRANSFORMATION_HPP

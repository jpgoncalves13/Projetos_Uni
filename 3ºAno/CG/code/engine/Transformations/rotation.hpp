#ifndef ENGINE_ROTATION_H
#define ENGINE_ROTATION_H

#include "transformation.hpp"


class Rotation : public Transformation {
private:
    float angle;
    float startCounter;
    float duration360;
public:
    Rotation(float x, float y, float z, float angle, double startCounter, float duration360);
    float getAngle();
    void setAngle(float angle);
    void applyTransformation(float *matrix) override;
    void applyTransformationToPoint(Point* base, float* radius);
private:
    double getRotationAngle();
};


#endif //ENGINE_ROTATION_H

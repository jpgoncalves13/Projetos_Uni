#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H

#include <vector>
#include "transformation.hpp"

class Translation : public Transformation {
private:
    float duration;
    bool align;
    int tesselation;
    bool show;
    vector<Point> controlPoints;
    Point yi;
    float startCounter;
public:
    Translation(float x, float y, float z, float duration, bool align, int tesselation, bool show, vector<Point> controlPoints);
    void applyTransformation(float *matrix) override;
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
    void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float *pos, float *deriv);
    void renderCatmullRomCurve();
private:
    float getTranslationT();
};

#endif //ENGINE_TRANSLATION_H

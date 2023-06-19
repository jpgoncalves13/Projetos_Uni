//
// Created by mike on 10-05-2023.
//

#ifndef ENGINE_LIGHTDIRECTIONAL_H
#define ENGINE_LIGHTDIRECTIONAL_H


#include "Light.h"

class LightDirectional : public Light {
private:
public:
    LightDirectional(float dirX, float dirY, float dirZ);
    void doAction(int light);
    float getDirX();
    float getDirY();
    float getDirZ();

private:
    float dirX, dirY, dirZ;
};


#endif //ENGINE_LIGHTDIRECTIONAL_H

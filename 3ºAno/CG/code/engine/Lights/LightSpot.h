//
// Created by mike on 10-05-2023.
//

#ifndef ENGINE_LIGHTSPOT_H
#define ENGINE_LIGHTSPOT_H


#include "Light.h"

class LightSpot : public Light {
public:
    LightSpot(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff);
    void doAction(int light);

    float getPosX();
    float getPosY();
    float getPosZ();

    float getDirX();
    float getDirY();
    float getDirZ();

    float getCutoff();

private:
    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    float cutoff;
};


#endif //ENGINE_LIGHTSPOT_H

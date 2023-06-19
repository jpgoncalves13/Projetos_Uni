//
// Created by mike on 10-05-2023.
//

#ifndef ENGINE_LIGHTPOINT_H
#define ENGINE_LIGHTPOINT_H


#include "Light.h"

class LightPoint : public Light {
public:
    LightPoint(float posX, float posY, float posZ);
    void doAction(int light);
    float getPosX();
    float getPosY();
    float getPosZ();

private:
    float posX, posY, posZ;

};



#endif //ENGINE_LIGHTPOINT_H

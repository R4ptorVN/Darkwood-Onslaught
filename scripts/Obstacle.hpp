#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"


class Obstacle : public Entity
{
    public:
        Obstacle(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        Entity getUpperHalf();
        Entity getLowerHalf();
    private:
        Entity upperHalf;
        Entity lowerHalf;
};
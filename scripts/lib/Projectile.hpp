#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Enemy.hpp"

class Projectile : public Enemy
{
    public:
        Projectile(int type, float srcX, float srcY, float desX, float desY);
        void updateEnemy(float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes);

    protected:
        SDL_Rect hitBox;

};

void setUpProjectileTexture(RenderWindow& window);
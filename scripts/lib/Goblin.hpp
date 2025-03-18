#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Enemy.hpp"

class Goblin : public Enemy
{
    public:
        Goblin(float desX, float desY);
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        void checkDamageEnemy(Player &player);
        void updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes);
};

void setUpGoblinTexture(RenderWindow& window);
#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Enemy.hpp"
#include "Projectile.hpp"
#include "Skeleton.hpp"

class Necromancer : public Enemy
{
    public:
        Necromancer(float desX, float desY);
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        pair<int, int> getLaserPos();
        void checkDamageEnemy(Player &player);
        void updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes, vector<Enemy*> &Enemies);

    protected:
        bool isAttacking;
        int summoningCooldown;
        int randomPortalValueX, randomPortalValueY;

};

void setUpNecromancerTexture(RenderWindow& window);
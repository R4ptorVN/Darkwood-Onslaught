#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"
#include "RenderWindow.hpp"


class Player: public Entity
{
    public:
        Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        float getHealthPoints();
        void setHealthPoints(float x);
        bool checkDeath();
        void setStateTexture(int x);
        bool isAttacking();
        float getAttackingDamage();
        void levelUp();
        SDL_Rect getHealthBar();
        SDL_Rect getHitBox();
        SDL_Rect getRenderBoxValues();
        SDL_Rect getSwordBox(int box);
        void moveCharacter();
        void updateFrame(float x, float y);
        bool outOfMap();
        void updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime, bool &gameRunning);

    protected:
        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown, attacking;

        int frame;

        int frameDuration;

        int state;

        int textureState;

        int movingDirection;
        int movingDirections;

        float healthPoints;

        float attackingDamage;

        SDL_Rect healthBar;

        float lastFrameTime;

};

Player setupPlayerTexture(RenderWindow& window);

void updateCamera(SDL_Rect &camera, Player &mainCharacter);
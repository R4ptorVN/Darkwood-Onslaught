#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"


class Player: public Entity
{
    public:
        Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        float getHealthPoints();
        void setHealthPoints(float x);
        bool checkDeath();
        SDL_Rect getHitBox();
        SDL_Rect getRenderBoxValues();
        void moveCharacter();
        void updateFrame(float x, float y);
        void updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime, bool &gameRunning);
        bool outOfMap();

    private:

        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown, attacking;

        int frame;

        int frameDuration;

        int state;

        float srcXFrames[20];

        float srcYFrames[4][4];

        int maxFrames[4];

        int movingDirection;
        int movingDirections;

        float healthPoints;

        float lastFrameTime;
};
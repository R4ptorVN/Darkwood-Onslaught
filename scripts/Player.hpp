#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"


class Player: public Entity
{
    public:
        Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        SDL_Rect getLegFrame();
        SDL_Rect getChestFrame();
        void moveCharacter();
        void updateFrame(float x, float y);
        void updateMovement(vector<Entity> &ObstaclesLower, vector<Entity> &ObstaclesUpper, Entity &map, float currentFrameTime);
        bool outOfMap(Entity &map);

    private:

        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown, attacking;

        int frame;

        float srcXFrames[20];

        float srcYIdleFrames[4];

        float srcYWalkingFrames[4];

        float srcYAttackingFrames[4];

        int movingDirection;
        int movingDirections;

        float lastFrameTime;
};
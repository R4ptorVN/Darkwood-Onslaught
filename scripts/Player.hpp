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
        void updateFrame(float x, float y, float h, float w, bool flip);
        void updateMovement(vector<Entity> &ObstaclesLower, vector<Entity> &ObstaclesUpper, float currentFrameTime);

    private:

        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown, attacking;

        int frame;

        float srcXIdleFrames[12];
        float srcYIdleFrames[12];
        float srcWIdleFrames[12];
        float srcHIdleFrames[12];

        float srcXWalkingFrames[12];
        float srcYWalkingFrames[12];
        float srcWWalkingFrames[12];
        float srcHWalkingFrames[12];

        float srcXAttackingFrames[12];
        float srcYAttackingFrames[12];
        float srcWAttackingFrames[12];
        float srcHAttackingFrames[12];

        bool facingLeft;
        int movingDirections;

        float lastFrameTime;
};
#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"


class Player: public Entity
{
    public:
        Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, vector<SDL_Texture*> textures);
        SDL_Rect getLegFrame();
        SDL_Rect getChestFrame();
        void moveCharacter();
        void updateFrame(SDL_Texture* tex, float x, float y, float h, float w, bool flip);
        void updateMovement(vector<Entity> &ObstaclesLower, vector<Entity> &ObstaclesUpper, float currentFrameTime);

    private:
        SDL_Texture* girlIdle;
        SDL_Texture* girlWalk;
        SDL_Texture* girlAttack;

        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown, attacking;

        int frame;

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
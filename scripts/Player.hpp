#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Entity.hpp"


class Player: public Entity
{
public:
    Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, vector<SDL_Texture*> textures);
    SDL_Rect getLegFrame();
    void moveCharacter();
    void updateFrame(SDL_Texture* tex, float x, float y, bool flip);
    void updateMovement(SDL_Event &event, vector<Entity> &Obstacles);
private:

    SDL_Texture* girlIdle;
    SDL_Texture* girlWalk;

    float movementSpeed;
    bool movingLeft, movingRight, movingUp, movingDown;

    int frame;
    float srcXFrames[12];
    float srcYFrames[12];

    bool facingLeft;
    int movingDirections;
};
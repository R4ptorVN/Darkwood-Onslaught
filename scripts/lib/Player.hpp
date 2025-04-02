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
        void increaseMana();
        bool checkDeath();
        void setStateTexture(int x);
        int getAttackingState();
        float getAttackingDamage();
        void levelUp(int wave);
        SDL_Rect getHealthBar();
        SDL_Rect getManaBar();
        SDL_Rect getHitBox();
        SDL_Rect getRenderBoxValues();
        SDL_Rect getSwordBox(int box);
        SDL_Rect getSkillBox(int box);
        void moveCharacter();
        void updateFrame(float x, float y);
        bool outOfMap();
        void updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime, bool &gameEnding);

    protected:
        float movementSpeed;
        bool movingLeft, movingRight, movingUp, movingDown;

        int attackingState;

        int frame;

        int frameDuration;

        int state;

        int textureState;

        int movingDirection;
        int movingDirections;

        float healthPoints;
        float manaPoints;

        float attackingDamage;

        float lastFrameTime;

        int frameDistance[5];
};

Player setupPlayerTexture(RenderWindow& window);

void updateCamera(SDL_Rect &camera, Player &mainCharacter);
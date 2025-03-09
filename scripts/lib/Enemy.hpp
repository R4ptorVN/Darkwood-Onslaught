#pragma once

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Functions.hpp"

using namespace std;

class Enemy : public Entity
{
    public:
        Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        void moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);
        SDL_Rect getRenderBoxValues();
        SDL_Rect getHitBox();
        void updateFrame(float x, float y, float w, float h);

     private:
        float movementSpeed;

        int attackCooldown;

        int frameDuration;

        int state;

        bool facingLeft;

        int frame;
        float lastFrameTime;
        float lastFramePos;

        int maxFrames[4];

        float srcXFrames[4][8];
        float srcYFrames[4];
        float srcWFrames[4];
        float srcHFrames[4];
};

void setupEnemyTexture(RenderWindow& window);

void buildEnemies(float currentFrameTime);

vector<Enemy> getEnemies();

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void checkContactEnemies(Player &player);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);

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


     private:
        float movementSpeed;

        int frame;
        float lastFrameTime;

        float lastFramePos;

        int actionStatus;

        float srcXFrames[8];
};

void setupEnemyTexture(RenderWindow& window);

void buildEnemies();

vector<Enemy> getEnemies();

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);

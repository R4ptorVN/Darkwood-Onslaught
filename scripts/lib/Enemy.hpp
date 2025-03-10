#pragma once

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Functions.hpp"

using namespace std;

class Enemy : public Entity
{
    public:
        Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        int getFrameDuration();
        float getHealthPoints();
        void setHealthPoints(float x);
        bool checkDeath();
        void moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);
        SDL_Rect getRenderBoxValues();
        SDL_Rect getHitBox();
        void updateFrame(float x, float y, float w, float h);
        void checkDamageEnemy(Player &player);

     private:
        float movementSpeed;

        int attackCooldown;

        int frameDuration;

        int randomValueX;
        int randomValueY;
        float targetX = 0, targetY = 0;

        float healthPoints;

        bool takingDamage;

        int state;

        bool facingLeft;

        int frame;
        float lastFrameTime;
        float lastFramePos;

        int maxFrames[4];

        float srcXFrames[4][20];
        float srcYFrames[4];
        float srcWFrames[4];
        float srcHFrames[4];
};

void setupEnemyTexture(RenderWindow& window);

void buildEnemies(float currentFrameTime);

vector<Enemy> getEnemies();

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void checkContactEnemies(Player &player);

void checkDamageEnemies(Player& player);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);

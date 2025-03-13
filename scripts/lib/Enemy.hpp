#pragma once

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Functions.hpp"

using namespace std;

class Enemy : public Entity
{
    public:
        Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        int getState();
        int getFrameDuration();
        float getHealthPoints();
        void setHealthPoints(float x);
        bool checkDeath();
        void moveEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);
        SDL_Rect getRenderBoxValues();
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        void updateFrame(float x, float y, float w, float h);
        void checkDamageEnemy(Player &player);

     private:
        float movementSpeed;

        int actionCooldown;

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

        int maxFrames[5];

        float srcXFrames[5][20];
        float srcYFrames[5];
        float srcWFrames[5];
        float srcHFrames[5];
};

void setupEnemyTexture(RenderWindow& window);

int getWave();

void buildEnemies(float currentFrameTime);

vector<Enemy> getEnemies();

void moveEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void checkDamageEnemies(Player& player);

void checkContactPlayer(Player &player);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);

#pragma once

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Functions.hpp"

using namespace std;

class Enemy : public Entity
{
    public:
        virtual ~Enemy() = default;
        Enemy(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex);
        int getState();
        int getFrameDuration();
        float getHealthPoints();
        void setHealthPoints(float x);
        bool checkDeath();
        SDL_Rect getRenderBoxValues();
        void updateFrame(float x, float y, float w, float h);
        void checkDamageEnemy(Player &player);
        void checkSkill(SDL_Rect hitBox, Player &player);
        void moveEnemy(vector<Entity> &Obstacles);
        void knockBackEnemy(vector<Entity> &Obstacles);

     protected:
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

        int knockBack;

        int frame;
        float lastFrameTime;
        float lastFramePos;

        int maxFrames[5];

        float srcXFrames[5][35];
        float srcYFrames[5];
        float srcWFrames[5];
        float srcHFrames[5];

        SDL_Rect renderBox[5];
};

void setupEnemyTexture(RenderWindow& window);

void newWave();

int getWave();

void spawnEnemies(float currentFrameTime);

vector<Enemy> getEnemies();

void updateEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void checkDamageEnemies(Player& player);

void checkContactPlayer(Player &player);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);

class Projectile : public Enemy
{
    public:
        Projectile(int type, float srcX, float srcY, float desX, float desY);
        void updateEnemy(float currentFrameTime);

    protected:
        SDL_Rect hitBox;

};

class Skeleton : public Enemy
{
    public:
        Skeleton(float desX, float desY);
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        void checkDamageEnemy(Player &player);
        void updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

};

class Goblin : public Enemy
{
    public:
        Goblin(float desX, float desY);
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        void checkDamageEnemy(Player &player);
        void updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);
};

class Orc : public Enemy
{
    public:
        Orc(float desX, float desY);
        SDL_Rect getHitBox();
        SDL_Rect getBodyBox();
        void checkDamageEnemy(Player &player);
        void updateEnemy(Player &player, vector<Entity> &Obstacles, float currentFrameTime);
};
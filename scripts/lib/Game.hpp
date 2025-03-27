#pragma once
#include<bits/stdc++.h>

using namespace std;

#include "Enemy.hpp"
#include "Projectile.hpp"
#include "Skeleton.hpp"
#include "Goblin.hpp"
#include "Orc.hpp"
#include "Necromancer.hpp"

void setupEnemyTexture(RenderWindow& window);

void newWave(int curWave);

int getWave();

void addEnemy(Enemy newEnemy);

void spawnEnemies(float currentFrameTime);

vector<Enemy> getEnemies();

void updateEnemies(Player &player, vector<Entity> &Obstacles, float currentFrameTime);

void checkDamageEnemies(Player& player);

void checkContactPlayer(Player &player);

void renderEnemies(RenderWindow& window, SDL_Rect &camera);



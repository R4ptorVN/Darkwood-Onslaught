#pragma once

#include "Player.hpp"

using namespace std;

const int LEVEL_WIDTH = 512 * 2;
const int LEVEL_HEIGHT = 384 * 2;
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 384;

extern mt19937 mt;

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles);
void updateCamera(SDL_Rect &camera, Player &mainCharacter);



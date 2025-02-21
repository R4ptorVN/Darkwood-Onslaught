#pragma once

#include "Player.hpp"

using namespace std;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 720;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles);
void updateCamera(SDL_Rect &camera, Player &mainCharacter);


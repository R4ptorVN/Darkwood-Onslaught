#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include<bits/stdc++.h>

#include"Entity.hpp"

using namespace std;

const int LEVEL_WIDTH = 512 * 2;
const int LEVEL_HEIGHT = 384 * 2;
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 384;

extern mt19937 mt;

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles);

SDL_Rect makeRec(int x, int y, int w, int h);



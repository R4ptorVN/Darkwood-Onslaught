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

class Button
{
    public:
        Button(int x, int y, int w, int h);
        SDL_Rect getButtonRec();
        void setButtonState(int x);
        bool getButtonState();

    protected:
        SDL_Rect buttonRec;
        bool buttonState;
};


void setUpButtons(SDL_Rect &playButton, SDL_Rect &retryButton);

extern mt19937 mt;

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles);

bool checkInside(int x, int y, SDL_Rect rec);

SDL_Rect makeRec(int x, int y, int w, int h);



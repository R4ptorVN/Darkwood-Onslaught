#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include<bits/stdc++.h>

using namespace std;

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Obstacle.hpp"

class Map
{
    public:
        Map(RenderWindow &window);
        void renderLayer(RenderWindow &window, SDL_Rect &camera, int Layer);
        vector<Entity>& getHitBoxes();
        vector<SDL_Rect>& getDisplayBoxesValues();
        Entity& getFire(float currentFrameTime);

    protected:
        Entity mapLayer[4];

        vector<Entity> obstacleHitBox;
        vector<SDL_Rect> obstacleDisplay;

        Entity Fire;
        float srcXFire[8];
        int fireFrame;

        float lastFrameTime;
};

SDL_Rect makeRec(int x, int y, int w, int h);

#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include<bits/stdc++.h>

using namespace std;

#include "RenderWindow.hpp"
#include "Player.hpp"

class Map
{
    public:
        Map(RenderWindow &window);
        void renderLayer(RenderWindow &window, SDL_Rect &camera, int Layer);
        void renderObjectsBack(RenderWindow &window, SDL_Rect &camera, Player &player, float currentFrameTime);
        void renderObjectsFront(RenderWindow &window, SDL_Rect &camera, Player &player, float currentFrameTime);
        vector<Entity>& getHitBoxes();

    private:
        Entity mapLayer[4];

        vector<Entity> obstacleHitBox;
        vector<Entity> obstacleDisplay;

        Entity Fire;
        float srcXFire[8];
        int fireFrame;

        float lastFrameTime;
};

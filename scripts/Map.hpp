#pragma once
#include<SDL.h>
#include<SDL_image.h>

#include<bits/stdc++.h>

using namespace std;

#include "RenderWindow.hpp"
#include "Entity.hpp"

class Map
{
    public:
        Map(RenderWindow &window);
        void render(RenderWindow &window, SDL_Rect &camera);

    private:
        SDL_Texture *Layer[4];
        Entity mapLayer[4];

        int fireFrame;
};

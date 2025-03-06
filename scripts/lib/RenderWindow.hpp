#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "Entity.hpp"

using namespace std;

class RenderWindow
{
    public:
        RenderWindow(const char* p_title);
        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        void init();
        void render(Entity& p_entity, SDL_Rect &camera);
        void clearEntities();
        void pushEntities(Entity& HitBox, SDL_Rect Values, float multiplier);
        void renderEntity(Entity& HitBox, SDL_Rect Values, float multiplier, SDL_Rect& camera);
        void renderEntities(SDL_Rect &camera);
        void display();
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        vector<pair<pair<Entity, SDL_Rect>, float> > Entities;
};
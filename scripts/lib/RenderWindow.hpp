#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <bits/stdc++.h>

#include "Functions.hpp"
#include "Entity.hpp"

using namespace std;

class RenderWindow
{
    public:
        RenderWindow(const char* p_title);
        SDL_Texture* loadTexture(const char* p_filePath);
        SDL_Texture* loadText(const char* source, const char* word, int size, SDL_Color color);
        void clear();
        void setUpHUD();
        void init();
        pair<int, int> getMousePosition(int realMouseX, int realMouseY);
        void render(Entity& p_entity, SDL_Rect &camera);
        void renderTitle(Entity& p_entity, SDL_Rect &camera, bool buttonEffect);
        void renderEnding(int wave, SDL_Rect &camera, bool buttonEffect);
        void setFade(int x);
        int screenFade();
        void clearEntities();
        void pushEntities(Entity& HitBox, SDL_Rect Values, float multiplier);
        void renderEntity(Entity& HitBox, SDL_Rect Values, float multiplier, SDL_Rect& camera);
        void renderEntities(SDL_Rect &camera);
        void renderBars(SDL_Rect &healthBar, SDL_Rect &manaBar);
        void renderWave(int wave);
        void display();
        void delay(float startTime);
        void cleanUp();

    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int fadeStatus;
        vector<pair<pair<Entity, SDL_Rect>, float> > Entities;
};
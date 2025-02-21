#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include "Entity.hpp"

class RenderWindow
{
    public:
        RenderWindow(const char* p_title);
        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        void init();
        void render(Entity& p_entity, SDL_Rect &camera);
        void display();
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};
#include <iostream>

#include "functions.hpp"

using namespace std;

#include "RenderWindow.hpp"
#include "Entity.hpp"

RenderWindow::RenderWindow(const char* p_title)
:window(NULL), renderer(NULL)
{
  window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LEVEL_WIDTH, LEVEL_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL)
      SDL_Log("Window failed to initialize");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        SDL_Log("Failed to load texture");

    return texture;
}

void RenderWindow::init()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity, SDL_Rect &camera)
{
    SDL_Rect src = p_entity.getSrcFrame();
    SDL_Rect dest = p_entity.getDestFrame();
    dest.x -= camera.x;
    dest.y -= camera.y;
    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dest, 0, NULL, p_entity.getFlip());
}

void RenderWindow::renderPlayer(Entity &p_entity, SDL_Rect &camera)
{
    SDL_Rect src = p_entity.getSrcFrame();
    SDL_Rect dest = p_entity.getDestFrame();

    dest.x -= camera.x;
    dest.y -= camera.y;

    src.x -= 26;
    src.y -= 16;
    src.w += 52;
    src.h += 32;

    dest.x -= (26 * 2);
    dest.y -= (16 * 2);
    dest.w += (52 * 2);
    dest.h += (32 * 2);

    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dest, 0, NULL, p_entity.getFlip());
}

void RenderWindow::display()
{
     SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

#include <iostream>

#include "lib/Functions.hpp"

using namespace std;

#include "lib/RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title)
:window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LEVEL_WIDTH, LEVEL_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
        SDL_Log("Window failed to initialize");

    Entities.clear();

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

SDL_Texture* healthBarFrame;
SDL_Texture* heartIcon;
void RenderWindow::setUpHealthBar()
{
     healthBarFrame = loadTexture("resources/Bar.png");
     heartIcon = loadTexture("resources/Heart.png");
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

void RenderWindow::clearEntities()
{
    Entities.clear();
}

void RenderWindow::pushEntities(Entity& HitBox, SDL_Rect Values, float multiplier)
{
    Entities.push_back({{HitBox, Values}, multiplier});
}

bool compare(pair<pair<Entity, SDL_Rect>, float> x, pair<pair<Entity, SDL_Rect>, float> y)
{
    return(x.first.first.getDesY() + x.first.first.getDesH() < y.first.first.getDesY() + y.first.first.getDesH());
}

void RenderWindow::renderEntity(Entity& HitBox, SDL_Rect Values, float multiplier, SDL_Rect& camera)
{
    SDL_Rect src = HitBox.getSrcFrame();
    SDL_Rect dest = HitBox.getDestFrame();

    dest.x -= camera.x;
    dest.y -= camera.y;

    src.x += Values.x;
    src.y += Values.y;
    src.w = Values.w;
    src.h = Values.h;

    dest.x += (Values.x * multiplier);
    dest.y += (Values.y * multiplier);
    dest.w = (Values.w * multiplier);
    dest.h = (Values.h * multiplier);

    SDL_RenderCopyEx(renderer, HitBox.getTex(), &src, &dest, 0, NULL, HitBox.getFlip());
}

void RenderWindow::renderEntities(SDL_Rect &camera)
{
    sort(Entities.begin(), Entities.end(), compare);

    for (int i = 0; i < int(Entities.size()); i++)
        renderEntity(Entities[i].first.first, Entities[i].first.second, Entities[i].second, camera);
}

void RenderWindow::renderHealthBar(SDL_Rect &healthBar, SDL_Rect &camera)
{
    healthBar.x = 30; healthBar.y = SCREEN_HEIGHT - 30;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
    SDL_RenderFillRect(renderer, &healthBar);
    SDL_RenderDrawRect(renderer, &healthBar);

    SDL_Rect srcFrame;
    srcFrame.x = 0; srcFrame.y = 0;
    srcFrame.w = 52; srcFrame.h = 9;

    SDL_Rect destFrame;
    destFrame.x = 30; destFrame.y = SCREEN_HEIGHT - 30;
    destFrame.w = 100; destFrame.h = 14;

    SDL_RenderCopyEx(renderer, healthBarFrame, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);

    srcFrame.x = 0; srcFrame.y = 0;
    srcFrame.w = 13; srcFrame.h = 12;

    destFrame.x = 12; destFrame.y = SCREEN_HEIGHT - 35;
    destFrame.w = 26; destFrame.h = 24;

    SDL_RenderCopyEx(renderer, heartIcon, &srcFrame, &destFrame, 0, NULL, SDL_FLIP_NONE);
}

void RenderWindow::display()
{
     SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
     SDL_DestroyWindow(window);
}

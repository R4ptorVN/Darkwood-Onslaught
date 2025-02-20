#include <SDL.h>
#include <SDL_image.h>
#include<iostream>

using namespace std;

#include "Entity.hpp"

Entity::Entity(int srcX, int srcY, int srcW, int srcH, float p_x, float p_y, SDL_Texture* p_tex, SDL_RendererFlip p_flip)
:x(p_x), y(p_y), tex(p_tex), flip(p_flip)
{
    currentFrame.x = srcX;
    currentFrame.y = srcY;
    currentFrame.w = srcW;
    currentFrame.h = srcH;
}

float Entity::getX()
{
    return x;
}

void Entity::setX(int val)
{
    x = val;
}

float Entity::getY()
{
    return y;
}

void Entity::setY(int val)
{
    y = val;
}

SDL_RendererFlip Entity::getFlip()
{
    return flip;
}

void Entity::setFlip(SDL_RendererFlip newFlip)
{
    flip = newFlip;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}

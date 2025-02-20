#include <SDL.h>
#include <SDL_image.h>
#include<iostream>

using namespace std;

#include "Entity.hpp"

Entity::Entity(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* p_tex)
:srcX(srcX), srcY(srcY), srcW(srcW), srcH(srcH), desX(desX), desY(desY), desW(desW), desH(desH), tex(p_tex)
{
    flip = SDL_FLIP_NONE;

    srcFrame.x = srcX;
    srcFrame.y = srcY;
    srcFrame.w = srcW;
    srcFrame.h = srcH;

    destFrame.x = desX;
    destFrame.y = desY;
    destFrame.w = desW;
    destFrame.h = desH;

}
float Entity::getSrcX()
{
    return srcX;
}

float Entity::getSrcY()
{
    return srcY;
}

float Entity::getSrcW()
{
    return srcW;
}

float Entity::getSrcH()
{
    return srcH;
}

float Entity::getDesX()
{
    return desX;
}

float Entity::getDesY()
{
    return desY;
}

float Entity::getDesW()
{
    return desW;
}

float Entity::getDesH()
{
    return desH;
}

void Entity::setSrcX(float val)
{
    srcX = val;
    srcFrame.x = srcX;
}

void Entity::setSrcY(float val)
{
    srcY = val;
    srcFrame.y = srcY;
}

void Entity::setSrcW(float val)
{
    srcW = val;
    srcFrame.w = srcW;
}

void Entity::setSrcH(float val)
{
    srcH = val;
    srcFrame.h = srcH;
}

void Entity::setDesX(float val)
{
    desX = val;
    destFrame.x = desX;
}

void Entity::setDesY(float val)
{
    desY = val;
    destFrame.y = desY;
}

void Entity::setDesW(float val)
{
    desW = val;
    destFrame.w = desW;
}

void Entity::setDesH(float val)
{
    desH = val;
    destFrame.h = desH;
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

void Entity::setTex(SDL_Texture *newTex)
{
    tex = newTex;
}

SDL_Rect Entity::getSrcFrame()
{
    return srcFrame;
}

SDL_Rect Entity::getDestFrame()
{
    return destFrame;
}
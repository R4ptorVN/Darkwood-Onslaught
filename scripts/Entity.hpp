#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Entity
{
public:
    Entity(int srcX, int srcY, int srcW, int srcH, float p_x, float p_y, float m_w, float m_h, SDL_Texture* p_tex, SDL_RendererFlip p_flip);
    float getX();
    float getY();
    float getW();
    float getH();
    void setX(int val);
    void setY(int val);
    SDL_RendererFlip getFlip();
    void setFlip(SDL_RendererFlip newFlip);
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
private:
    float x, y, w, h;
    SDL_Rect currentFrame;
    SDL_Texture* tex;
    SDL_RendererFlip flip;
};
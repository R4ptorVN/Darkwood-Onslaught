#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Entity
{
    public:
        Entity();
        Entity(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* p_tex);
        float getSrcX();
        float getSrcY();
        float getSrcW();
        float getSrcH();
        void setSrcX(float val);
        void setSrcY(float val);
        void setSrcW(float val);
        void setSrcH(float val);
        float getDesX();
        float getDesY();
        float getDesW();
        float getDesH();
        void setDesX(float val);
        void setDesY(float val);
        void setDesW(float val);
        void setDesH(float val);
        SDL_Texture* getTex();
        void setTex(SDL_Texture* newTex);
        SDL_RendererFlip getFlip();
        void setFlip(SDL_RendererFlip newFlip);
        SDL_Rect getSrcFrame();
        SDL_Rect getDestFrame();
    protected:
        float srcX, srcY, srcW, srcH;
        float desX, desY, desW, desH;
        SDL_Rect srcFrame, destFrame;
        SDL_Texture* tex;
        SDL_RendererFlip flip;
};
#include "Functions.hpp"

using namespace std;

bool checkCollision(SDL_Rect a, vector<Entity> &Obstacles)
{
        for (Entity &object : Obstacles)
        {
            SDL_Rect b = object.getDestFrame();
            if (SDL_HasIntersection(&a , &b))
                return true;
        }
        return false;
}

void updateCamera(SDL_Rect &camera, Player &mainCharacter)
{
        camera.x = mainCharacter.getDestFrame().x - SCREEN_WIDTH / 2;
        camera.y = mainCharacter.getDestFrame().y - SCREEN_HEIGHT / 2;
        camera.w = SCREEN_WIDTH;
        camera.h = SCREEN_HEIGHT;

        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > LEVEL_WIDTH - camera.w) camera.x = LEVEL_WIDTH - camera.w;
        if (camera.y > LEVEL_HEIGHT - camera.h) camera.y = LEVEL_HEIGHT - camera.h;
}


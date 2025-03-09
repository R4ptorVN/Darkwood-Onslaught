#include "lib/Functions.hpp"

using namespace std;

int Rand(int l, int r)
{
        return l + ((int)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                    (int)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                    (int)rand() * (RAND_MAX + 1) +
                    rand()) % (r - l + 1);
}

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
        camera.x = mainCharacter.getHitBox().x - SCREEN_WIDTH / 2;
        camera.y = mainCharacter.getHitBox().y - SCREEN_HEIGHT / 2;
        camera.w = SCREEN_WIDTH;
        camera.h = SCREEN_HEIGHT;

        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > LEVEL_WIDTH - SCREEN_WIDTH) camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
        if (camera.y > LEVEL_HEIGHT - SCREEN_HEIGHT) camera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}


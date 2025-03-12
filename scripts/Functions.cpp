#include "lib/Functions.hpp"

using namespace std;

mt19937 mt(chrono::steady_clock::now().time_since_epoch().count());

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

SDL_Rect makeRec(int x, int y, int w, int h)
{
    SDL_Rect rec;

    rec.x = x;
    rec.y = y;
    rec.w = w;
    rec.h = h;

    return rec;
}

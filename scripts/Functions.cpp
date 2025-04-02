#include "lib/Functions.hpp"

using namespace std;

Button::Button(int x, int y, int w, int h)
{
    buttonRec = makeRec(x, y, w, h);
    buttonState = false;
}

SDL_Rect Button::getButtonRec()
{
    return buttonRec;
}

void Button::setButtonState(int x)
{
    buttonState = x;
}

bool Button::getButtonState()
{
    return buttonState;
}

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

bool checkInside(int x, int y, SDL_Rect rec)
{
    if (x < rec.x || y < rec.y || x > rec.x + rec.w || y > rec.y + rec.h)
        return false;
    return true;
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

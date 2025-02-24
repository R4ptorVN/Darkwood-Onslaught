#include "lib/Obstacle.hpp"
#include "lib/Functions.hpp"

using namespace std;

Obstacle::Obstacle(float srcX, float srcY, float srcW, float srcH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, srcX * 2, srcY * 2, srcW * 2, srcH * 2, tex)
{

}



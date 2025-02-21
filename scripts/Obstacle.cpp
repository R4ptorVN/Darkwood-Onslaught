#include "Obstacle.hpp"
#include "Functions.hpp"

using namespace std;

Obstacle::Obstacle(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{
        upperHalf.clear();
        upperHalf.setTex(tex);
        upperHalf.setSrcX(0);
        upperHalf.setSrcY(0);
        upperHalf.setSrcW(srcW);
        upperHalf.setSrcH(srcH / 2);
        upperHalf.setDesX(desX);
        upperHalf.setDesY(desY);
        upperHalf.setDesW(desW);
        upperHalf.setDesH(desH / 2);

        lowerHalf.clear();
        lowerHalf.setTex(tex);
        lowerHalf.setSrcX(0);
        lowerHalf.setSrcY(srcH / 2);
        lowerHalf.setSrcW(srcW);
        lowerHalf.setSrcH(srcH / 2);
        lowerHalf.setDesX(desX);
        lowerHalf.setDesY(desY + desH / 2);
        lowerHalf.setDesW(desW);
        lowerHalf.setDesH(desH / 2);
}

Entity Obstacle::getUpperHalf()
{
        return upperHalf;
}

Entity Obstacle::getLowerHalf()
{
        return lowerHalf;
}
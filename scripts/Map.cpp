#include "Map.hpp"

using namespace std;

Map::Map(RenderWindow &window)
{
        Layer[0] = window.loadTexture("resources/mapLayer1.png");
        Layer[1] = window.loadTexture("resources/mapLayer2.png");
        Layer[2] = window.loadTexture("resources/mapLayer3.png");
        Layer[3] = window.loadTexture("resources/mapLayer4.png");

        for (int i = 0; i < 4; i++)
        {
                mapLayer[i].setTex(Layer[i]);
                mapLayer[i].setFlip(SDL_FLIP_NONE);
                mapLayer[i].setSrcX(0); mapLayer[i].setSrcY(0);
                mapLayer[i].setSrcW(512); mapLayer[i].setSrcH(384);
                mapLayer[i].setDesX(0); mapLayer[i].setDesY(0);
                mapLayer[i].setDesW(512 * 2); mapLayer[i].setDesH(384 * 2);
        }

        fireFrame = 0;
}

void Map::render(RenderWindow &window, SDL_Rect &camera)
{
        for (int i = 0; i < 4; i++)
                window.render(mapLayer[i], camera);
}
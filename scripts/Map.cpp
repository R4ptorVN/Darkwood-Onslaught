#include "lib/Map.hpp"
#include "lib/Obstacle.hpp"

using namespace std;

Map::Map(RenderWindow &window)
{
        SDL_Texture *Layer[4];
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

        //(69, 123, 19, 1) -> (38, 27, 80, 100);
        //(246, 90, 19, 1) -> (22, 0, 70, 93);
        //(166, 187, 18, 1) -> (133, 85, 88, 107);
        //(470, 219, 18, 1) -> (443, 125, 69, 99)
        //(310, 346, 19, 1) -> (276, 244, 87, 106);
        //(38, 266, 19, 1) -> (7, 169, 79, 101);
        //(256, 234, 15, 1) -> (256, 226, 16, 11);
        //(336, 126, 16, 1) -> (336, 112, 16, 16);
        //(356, 123, 7, 1) -> (355, 117, 9, 7)
        //(352, 111, 16, 1) -> (352, 97, 16, 15);
        //(368, 111, 16, 1) -> (368, 96, 16, 16);
        //(401, 111, 14, 1) -> (401, 97, 14, 15);

        obstacleHitBox.clear();
        obstacleHitBox.push_back(Obstacle(69, 123, 19, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(246, 90, 19, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(166, 187, 18, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(470, 219, 18, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(310, 346, 19, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(38, 266, 19, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(256, 234, 15, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(336, 126, 16, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(356, 123, 7, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(352, 111, 16, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(368, 111, 16, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(401, 111, 14, 1, Layer[2]));

        obstacleDisplay.clear();
        obstacleDisplay.push_back(Obstacle(38, 27, 80, 100, Layer[2]));
        obstacleDisplay.push_back(Obstacle(220, 0, 70, 93, Layer[2]));
        obstacleDisplay.push_back(Obstacle(133, 85, 88, 107, Layer[2]));
        obstacleDisplay.push_back(Obstacle(443, 125, 69, 99, Layer[2]));
        obstacleDisplay.push_back(Obstacle(276, 244, 87, 106, Layer[2]));
        obstacleDisplay.push_back(Obstacle(7, 169, 79, 101, Layer[2]));
        obstacleDisplay.push_back(Obstacle(256, 226, 16, 11, Layer[2]));
        obstacleDisplay.push_back(Obstacle(336, 112, 16, 16, Layer[2]));
        obstacleDisplay.push_back(Obstacle(355, 117, 9, 7, Layer[2]));
        obstacleDisplay.push_back(Obstacle(352, 97, 16, 15, Layer[2]));
        obstacleDisplay.push_back(Obstacle(368, 96, 16, 16, Layer[2]));
        obstacleDisplay.push_back(Obstacle(401, 97, 14, 15, Layer[2]));

        SDL_Texture *FireTex = window.loadTexture("resources/Fire.png");

        Fire.setTex(FireTex); Fire.setFlip(SDL_FLIP_NONE);
        Fire.setSrcX(0); Fire.setSrcY(0); Fire.setSrcW(16); Fire.setSrcH(23);
        Fire.setDesX(256 * 2); Fire.setDesY(208 * 2); Fire.setDesW(16 * 2); Fire.setDesH(23 * 2);


        float tmpX = 0;
        for (int i = 1; i < 8; i++)
              srcXFire[i] = srcXFire[i - 1] + 16;

        fireFrame = 0;

        lastFrameTime = 0;
}

vector<Entity>& Map::getHitBoxes()
{
        return obstacleHitBox;
}

void Map::renderLayer(RenderWindow &window, SDL_Rect &camera, int Layer)
{
        window.render(mapLayer[Layer], camera);
}

void Map::renderObjectsBack(RenderWindow &window, SDL_Rect &camera, Player &player, float currentFrameTime)
{
        for (int i = 0; i < int(obstacleHitBox.size()); i++)
        {
                if (player.getLegFrame().y > obstacleHitBox[i].getDesY())
                        window.render(obstacleDisplay[i], camera);
        }

        if (player.getLegFrame().y > Fire.getDesY() + Fire.getDesH())
        {
                if (currentFrameTime - lastFrameTime > 100)
                {
                        fireFrame++;
                        if (fireFrame == 8)
                                fireFrame = 0;
                        Fire.setSrcX(srcXFire[fireFrame]);

                        lastFrameTime = currentFrameTime;
                }

                window.render(Fire, camera);
        }
}

void Map::renderObjectsFront(RenderWindow &window, SDL_Rect &camera, Player &player, float currentFrameTime)
{
        for (int i = 0; i < int(obstacleHitBox.size()); i++)
        {
                if (player.getLegFrame().y <= obstacleHitBox[i].getDesY())
                        window.render(obstacleDisplay[i], camera);
        }

        if (player.getLegFrame().y <= Fire.getDesY() + Fire.getDesH())
        {
                if (currentFrameTime - lastFrameTime > 100)
                {
                        fireFrame++;
                        if (fireFrame == 8)
                                fireFrame = 0;
                        Fire.setSrcX(srcXFire[fireFrame]);

                        lastFrameTime = currentFrameTime;
                }

                window.render(Fire, camera);
        }
}
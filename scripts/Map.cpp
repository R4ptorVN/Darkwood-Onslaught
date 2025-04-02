#include "lib/Map.hpp"

using namespace std;

SDL_Texture* FireTex;

Map::Map(RenderWindow &window)
{
        SDL_Texture *Layer[4];
        Layer[0] = window.loadTexture("resources/Map/mapLayer1.png");
        Layer[1] = window.loadTexture("resources/Map/mapLayer2.png");
        Layer[2] = window.loadTexture("resources/Map/mapLayer3.png");
        Layer[3] = window.loadTexture("resources/Map/mapLayer4.png");

        FireTex = window.loadTexture("resources/Map/Fire.png");

        for (int i = 0; i < 4; i++)
        {
                mapLayer[i].setTex(Layer[i]);
                mapLayer[i].setFlip(SDL_FLIP_NONE);
                mapLayer[i].setSrcX(0); mapLayer[i].setSrcY(0);
                mapLayer[i].setSrcW(512); mapLayer[i].setSrcH(384);
                mapLayer[i].setDesX(0); mapLayer[i].setDesY(0);
                mapLayer[i].setDesW(512 * 2); mapLayer[i].setDesH(384 * 2);
        }

        //HitBox -> Display
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
        //(80, 335, 32, 1) -> (62, 287, 66, 51);

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
        obstacleHitBox.push_back(Obstacle(368, 110, 16, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(401, 111, 14, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(63, 334, 64, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(438, 106, 19, 1, Layer[2]));
        obstacleHitBox.push_back(Obstacle(438, 105, 19, 1, Layer[2]));

        obstacleHitBox.push_back(Obstacle(112, 344, 16, 7, Layer[3]));
        obstacleHitBox.push_back(Obstacle(368, 111, 16, 1, Layer[3]));
        obstacleHitBox.push_back(Obstacle(389, 264, 4, 1, Layer[3]));
        obstacleHitBox.push_back(Obstacle(80, 335, 16, 1, Layer[3]));
        obstacleHitBox.push_back(Obstacle(-1, -1, 0, 0, Layer[3]));
        //No Display Hit Box:

        obstacleHitBox.push_back(Obstacle(68, 293, 37, 13, Layer[2]));
        obstacleHitBox.push_back(Obstacle(64, 306, 50, 12, Layer[2]));
        obstacleHitBox.push_back(Obstacle(114, 312, 10, 7, Layer[2]));
        obstacleHitBox.push_back(Obstacle(119, 319, 4, 9, Layer[2]));
        obstacleHitBox.push_back(Obstacle(64, 319, 64, 16, Layer[2]));

        //Display:

        obstacleDisplay.clear();
        obstacleDisplay.push_back(makeRec(38, 27, 80, 100));
        obstacleDisplay.push_back(makeRec(220, 0, 70, 93));
        obstacleDisplay.push_back(makeRec(133, 85, 88, 107));
        obstacleDisplay.push_back(makeRec(443, 125, 69, 99));
        obstacleDisplay.push_back(makeRec(276, 244, 87, 106));
        obstacleDisplay.push_back(makeRec(7, 169, 79, 101));
        obstacleDisplay.push_back(makeRec(256, 226, 16, 11));
        obstacleDisplay.push_back(makeRec(336, 112, 16, 16));
        obstacleDisplay.push_back(makeRec(355, 117, 9, 7));
        obstacleDisplay.push_back(makeRec(352, 97, 16, 15));
        obstacleDisplay.push_back(makeRec(368, 96, 16, 16));
        obstacleDisplay.push_back(makeRec(401, 97, 14, 15));
        obstacleDisplay.push_back(makeRec(62, 287, 66, 51));
        obstacleDisplay.push_back(makeRec(432, 64, 32, 48));
        obstacleDisplay.push_back(makeRec(400, 0, 96, 64));

        obstacleDisplay.push_back(makeRec(112, 336, 16, 16));
        obstacleDisplay.push_back(makeRec(368, 96, 16, 16));
        obstacleDisplay.push_back(makeRec(384, 240, 16, 32));
        obstacleDisplay.push_back(makeRec(64, 288, 48, 48));
        obstacleDisplay.push_back(makeRec(96, 336, 16, 16));

        for (int i = 0; i < int(obstacleDisplay.size()); i++)
        {
                obstacleDisplay[i].x -= obstacleHitBox[i].getSrcX();
                obstacleDisplay[i].y -= obstacleHitBox[i].getSrcY();
        }

        Fire.setTex(FireTex); Fire.setFlip(SDL_FLIP_NONE);
        Fire.setSrcX(0); Fire.setSrcY(0); Fire.setSrcW(16); Fire.setSrcH(29);
        Fire.setDesX(256 * 2); Fire.setDesY(208 * 2); Fire.setDesW(16 * 2); Fire.setDesH(29 * 2);


        srcXFire[0] = 0;
        for (int i = 1; i < 8; i++)
              srcXFire[i] = srcXFire[i - 1] + 16;

        fireFrame = 0;

        lastFrameTime = 0;
}

Entity& Map::getFire(float currentFrameTime, bool gameEnding)
{
        if (!gameEnding && currentFrameTime - lastFrameTime > 100)
        {
                fireFrame++;
                if (fireFrame == 8)
                        fireFrame = 0;

                Fire.setSrcX(srcXFire[fireFrame]);

                lastFrameTime = currentFrameTime;
        }
        return Fire;
}

vector<Entity>& Map::getHitBoxes()
{
        return obstacleHitBox;
}

vector<SDL_Rect>& Map::getDisplayBoxesValues()
{
        return obstacleDisplay;
}

void Map::renderLayer(RenderWindow &window, SDL_Rect &camera, int Layer)
{
        window.render(mapLayer[Layer], camera);
}

#include "lib/Projectile.hpp"

using namespace std;

SDL_Texture* projectileTexture[1];

void setUpProjectileTexture(RenderWindow& window)
{
     projectileTexture[0] = window.loadTexture("resources/Enemies/Orc/orcSpike.png");
}

Projectile::Projectile(int type, float srcX, float srcY, float desX, float desY)
:Enemy(srcX, srcY, 0, 0, desX, desY, 0, 0, NULL)
{
    if (type == 1)
    {
        maxFrames[4] = 35;

        srcXFrames[4][0] = srcX;
        srcYFrames[4] = srcY;
        srcWFrames[4] = 10;
        srcHFrames[4] = 3;

        for (int i = 1; i < maxFrames[4]; i++)
            srcXFrames[4][i] = srcXFrames[4][i - 1] + 99;

        renderBox[4] = makeRec(0, -34, 10, 37);


        hitBox.x = desX + (1 * 1.25);
        hitBox.y = desY - (22 * 1.25);
        hitBox.w = 8 * 1.25;
        hitBox.h = 12 * 1.25;
    }

    setTex(projectileTexture[type - 1]);

    state = 4;

    frameDuration = maxFrames[4];
}

void Projectile::updateEnemy(float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes)
{
    if (currentFrameTime - lastFrameTime > 130)
    {
        if (frameDuration > 0)
            frameDuration--;

        frame++;
        frame %= maxFrames[state];

        updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);


        lastFrameTime = currentFrameTime;
    }

    if (frame > 6 && frame < 16)
        damageBoxes.push_back({hitBox, 0.5});
}
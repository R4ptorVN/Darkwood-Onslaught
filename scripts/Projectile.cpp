#include "lib/Projectile.hpp"

using namespace std;

SDL_Texture* projectileTexture[4];

void setUpProjectileTexture(RenderWindow& window)
{
    projectileTexture[0] = window.loadTexture("resources/Enemies/Orc/orcSpike.png");
    projectileTexture[1] = window.loadTexture("resources/Enemies/Necromancer/necromancerLaser.png");
    projectileTexture[2] = window.loadTexture("resources/Enemies/Necromancer/necromancerLaserFlipped.png");
    projectileTexture[3] = window.loadTexture("resources/Enemies/Necromancer/necromancerPortal.png");
}

Projectile::Projectile(int type, float srcX, float srcY, float desX, float desY)
:Enemy(srcX, srcY, 0, 0, desX, desY, 0, 0, NULL)
{
    projectileType = type;
    if (projectileType == 1)
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

    if (type == 2 || type == 3)
    {
        maxFrames[4] = 10;

        srcXFrames[4][0] = srcX;
        srcYFrames[4] = srcY;
        srcWFrames[4] = 32;
        srcHFrames[4] = 3;

        for (int i = 1; i < maxFrames[4]; i++)
            srcXFrames[4][i] = srcXFrames[4][i - 1] + 480;

        renderBox[4] = makeRec(0, -48, 480, 51);

        hitBox.x = desX + (9 * 1.25);
        hitBox.y = desY - (29 * 1.25);
        hitBox.w = 455 * 1.25;
        hitBox.h = 13 * 1.25;
    }

    if (type == 4)
    {
        maxFrames[4] = 16;

        srcXFrames[4][0] = srcX;
        srcYFrames[4] = srcY;
        srcWFrames[4] = 48;
        srcHFrames[4] = 1;

        for (int i = 1; i < maxFrames[4]; i++)
            srcXFrames[4][i] = srcXFrames[4][i - 1] + 144;

        renderBox[4] = makeRec(-48, 0, 144, 144);
    }

    setTex(projectileTexture[projectileType - 1]);

    state = 4;

    frameDuration = maxFrames[4];
}

void Projectile::updateEnemy(float currentFrameTime, vector<pair<SDL_Rect, float> > &damageBoxes)
{
    switch (projectileType)
    {
        case 1:
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
                damageBoxes.push_back({hitBox, 0.25});

            break;
        }

        case 2:
        {
            if (currentFrameTime - lastFrameTime > 90)
            {
                if (frameDuration > 0)
                    frameDuration--;

                frame++;
                frame %= maxFrames[state];

                updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);

                lastFrameTime = currentFrameTime;
            }

            if (frame > 0 && frame < 4)
                damageBoxes.push_back({hitBox, 1});

            break;
        }

        case 3:
        {
            if (currentFrameTime - lastFrameTime > 90)
            {
                if (frameDuration > 0)
                    frameDuration--;

                frame++;
                frame %= maxFrames[state];

                updateFrame(srcXFrames[state][frame], srcYFrames[state], srcWFrames[state], srcHFrames[state]);

                lastFrameTime = currentFrameTime;
            }

            if (frame > 0 && frame < 4)
                damageBoxes.push_back({hitBox, 1});

            break;
        }

        case 4:
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

            break;
        }
    }
}
#include<iostream>

using namespace std;

#include "lib/Functions.hpp"


const float frameDistance = 64;

float srcXFrames[20];

float srcYFrames[4][4];

int maxFrames[4];

SDL_Rect swordFrame[4];

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{

    movementSpeed = 2.5;

    healthPoints = 100;

    healthBar.x = 0; healthBar.y = 0;
    healthBar.w = 100; healthBar.h = 14;

    movingLeft = movingRight = movingUp = movingDown = false;

    attacking = false;

    frame = 0;

    frameDuration = 0;

    state = 0;

    textureState = 0;

    movingDirection = 0;
    movingDirections = 0;

    lastFrameTime = 0;

    maxFrames[0] = 12;
    maxFrames[1] = 8;
    maxFrames[2] = 6;
    maxFrames[3] = 14;

    srcXFrames[0] = 26;
    for (int i = 1; i < 20; i++)
        srcXFrames[i] = srcXFrames[i - 1] + frameDistance;

    srcYFrames[0][0] = 109;
    srcYFrames[0][1] = 493;
    srcYFrames[0][2] = 877;
    srcYFrames[0][3] = 1261;

    srcYFrames[1][0] = 237;
    srcYFrames[1][1] = 622;
    srcYFrames[1][2] = 1005;
    srcYFrames[1][3] = 1389;

    srcYFrames[2][0] = 365;
    srcYFrames[2][1] = 749;
    srcYFrames[2][2] = 1133;
    srcYFrames[2][3] = 1517;

    srcYFrames[3][0] = 1581;
    srcYFrames[3][1] = 1581;
    srcYFrames[3][2] = 1581;
    srcYFrames[3][3] = 1581;

    swordFrame[0].x = 18; swordFrame[0].y = -17;
    swordFrame[0].w = 20; swordFrame[0].h = 4;

    swordFrame[1].x = -2; swordFrame[1].y = 3;
    swordFrame[1].w = 4; swordFrame[1].h = 14;

    swordFrame[2].x = -25; swordFrame[2].y = -16;
    swordFrame[2].w = 16; swordFrame[2].h = 4;

    swordFrame[3].x = 11; swordFrame[3].y = -45;
    swordFrame[3].w = 4; swordFrame[3].h = 16;
}

SDL_Texture* playerTexture[2];

Player setupPlayerTexture(RenderWindow& window)
{
    playerTexture[0] = window.loadTexture("resources/warrior.png");
    playerTexture[1] = window.loadTexture("resources/warriorTakeDamage.png");

    Player player(26, 109, 12, 3, 300, 500, 12 * 2, 3 * 2, playerTexture[0]);
    return player;
}

float Player::getHealthPoints()
{
    return healthPoints;
}

void Player::setHealthPoints(float x)
{
    healthPoints = x;
}

bool Player::checkDeath()
{
    if (healthPoints <= 0)
        return true;
    if (healthPoints < 100)
        healthPoints += 0.01;
    return false;
}

void Player::setStateTexture(int x)
{
    textureState = x;
}

bool Player::isAttacking()
{
    return attacking;
}

SDL_Rect Player::getHealthBar()
{
    healthBar.w = max(0, int(healthPoints));
    return healthBar;
}

SDL_Rect Player::getHitBox()
{
    SDL_Rect hitBox = getDestFrame();

    hitBox.y -= (29 * 2);
    hitBox.h = (32 * 2);

    return hitBox;
}

SDL_Rect Player::getRenderBoxValues()
{
    SDL_Rect renderBox;

    renderBox.x = -26;
    renderBox.y = -45;
    renderBox.h = 64;
    renderBox.w = 64;

    return renderBox;
}

SDL_Rect Player::getSwordBox()
{
    SDL_Rect swordBox = getDestFrame();

    swordBox.x += (swordFrame[movingDirection].x * 2);
    swordBox.y += (swordFrame[movingDirection].y * 2);
    swordBox.w = swordFrame[movingDirection].w * 2;
    swordBox.h = swordFrame[movingDirection].h * 2;

    return swordBox;
}


void Player::moveCharacter()
{
    if (movingLeft)
        setDesX(getDesX() - movementSpeed);
    if (movingUp)
        setDesY(getDesY() - movementSpeed);
    if (movingRight)
        setDesX(getDesX() + movementSpeed);
    if (movingDown)
        setDesY(getDesY() + movementSpeed);
}

void Player::updateFrame(float x, float y)
{
    setSrcX(x);
    setSrcY(y);
}

bool Player::outOfMap()
{
     SDL_Rect dest = getDestFrame();
     if (dest.x < 0 || dest.y < 0 || dest.x + dest.w > LEVEL_WIDTH || dest.y + dest.h > LEVEL_HEIGHT)
         return true;
    return false;
}

void Player::updatePlayerMovement(vector<Entity> &Obstacles, float currentFrameTime, bool &gameRunning)
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    setTex(playerTexture[textureState]);

    if (frameDuration == 0)
    {
        if (checkDeath())
        {
            state = 3;
            frameDuration = maxFrames[state];

            frame = 0;
            updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);
            if (movingDirection == 2)
                setFlip(SDL_FLIP_HORIZONTAL);
        }
        else if (keyState[SDL_SCANCODE_J])
        {
            state = 2;
            frameDuration = maxFrames[state];

            frame = 0;
            updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);
        }
        else
        {
            movingLeft = keyState[SDL_SCANCODE_A];
            movingRight = keyState[SDL_SCANCODE_D];
            movingUp = keyState[SDL_SCANCODE_W];
            movingDown = keyState[SDL_SCANCODE_S];

            movingDirections = 0;

            if (movingDown)
            {
                movingDirection = 1;
                movingDirections++;
            }

            if (movingUp)
            {
                movingDirection = 3;
                movingDirections++;
            }

            if (movingLeft)
            {
                movingDirection = 2;
                movingDirections++;
            }

            if (movingRight)
            {
                movingDirection = 0;
                movingDirections++;
            }

            if (movingDirections > 0)
                state = 1;
            else
                state = 0;
        }
    }

    if (currentFrameTime - lastFrameTime > 100)
    {
        if (frameDuration > 0)
            frameDuration--;

        frame++;

        if (state == 3 && frame == maxFrames[state])
            frame--;
        else
            frame %= maxFrames[state];

        updateFrame(srcXFrames[frame], srcYFrames[state][movingDirection]);

        lastFrameTime = currentFrameTime;
    }

    switch (state)
    {
            case 1:
            {
                if (movingDirections > 1)
                    movementSpeed = 1.5;
                else
                    movementSpeed = 2.5;

                float prev_desX = desX;
                float prev_desY = desY;

                moveCharacter();

                if (checkCollision(getDestFrame(), Obstacles) || outOfMap())
                {
                    setDesX(prev_desX);
                    setDesY(prev_desY);
                }

                break;
            }

            case 2:
            {
                if (frame == 1)
                    attacking = true;
                else
                    attacking = false;

                break;
            }

            case 3:
            {
                 if (frameDuration == 0)
                     gameRunning = false;

                break;
            }
    }
}

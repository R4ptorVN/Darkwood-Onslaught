#include<iostream>

using namespace std;

#include "lib/Player.hpp"
#include "lib/Functions.hpp"

const float frameDistance = 64;

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{

    movementSpeed = 2.5;

    healthPoints = 100;

    movingLeft = movingRight = movingUp = movingDown = false;

    attacking = false;

    frame = 0;

    frameDuration = 0;

    state = 0;

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
    if (getHealthPoints() <= 0)
        return true;
    if (healthPoints < 100)
        healthPoints += 0.01;
    return false;
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

            case 3:
            {
                 if (frameDuration == 0)
                     gameRunning = false;

                break;
            }
    }
}

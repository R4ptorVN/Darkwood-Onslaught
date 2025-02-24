#include<iostream>

using namespace std;

#include "lib/Player.hpp"
#include "lib/Functions.hpp"

const int idleFrames = 12;
const int walkingFrames = 8;
const int attackingFrames = 6;

const float frameDistance = 64;

Player::Player(float srcX, float srcY, float srcW, float srcH, float desX, float desY, float desW, float desH, SDL_Texture* tex)
:Entity(srcX, srcY, srcW, srcH, desX, desY, desW, desH, tex)
{

    movementSpeed = 3;

    movingLeft = movingRight = movingUp = movingDown = false;

    attacking = false;

    frame = 0;

    movingDirection = 0;
    movingDirections = 0;

    lastFrameTime = 0;

    srcXFrames[0] = 26;
    for (int i = 1; i < 20; i++)
        srcXFrames[i] = srcXFrames[i - 1] + frameDistance;

    srcYIdleFrames[0] = 80;
    srcYIdleFrames[1] = 464;
    srcYIdleFrames[2] = 848;
    srcYIdleFrames[3] = 1232;

    srcYWalkingFrames[0] = 208;
    srcYWalkingFrames[1] = 593;
    srcYWalkingFrames[2] = 976;
    srcYWalkingFrames[3] = 1360;

    srcYAttackingFrames[0] = 336;
    srcYAttackingFrames[1] = 720;
    srcYAttackingFrames[2] = 1104;
    srcYAttackingFrames[3] = 1488;

}

SDL_Rect Player::getLegFrame()
{
    SDL_Rect legFrame = getDestFrame();

    legFrame.y += (29 * 2);
    legFrame.w = (12 * 2);
    legFrame.h = (3 * 2);

    return legFrame;
}

// SDL_Rect Player::getChestFrame()
// {
//     SDL_Rect chestFrame = getDestFrame();
//
//     chestFrame.h /= 3;
//
//     return chestFrame;
// }

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

void Player::updateMovement(vector<Entity> &ObstaclesLower, vector<Entity> &ObstaclesUpper, float currentFrameTime)
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    movingLeft = keyState[SDL_SCANCODE_A];
    movingRight = keyState[SDL_SCANCODE_D];
    movingUp = keyState[SDL_SCANCODE_W];
    movingDown = keyState[SDL_SCANCODE_S];

    if (keyState[SDL_SCANCODE_J] && !attacking)
    {
        attacking = true;
        frame = 0;
    }

    if (attacking)
    {
        updateFrame(srcXFrames[frame], srcYAttackingFrames[movingDirection]);
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == attackingFrames)
                attacking = false;

            lastFrameTime = currentFrameTime;
        }
        return;
    }

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
    {
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == walkingFrames)
                frame = 0;

            lastFrameTime = currentFrameTime;
        }
        updateFrame(srcXFrames[frame], srcYWalkingFrames[movingDirection]);
    }
    else
    {
        if (currentFrameTime - lastFrameTime > 100)
        {
            frame++;
            if (frame == walkingFrames)
                frame = 0;
            lastFrameTime = currentFrameTime;
        }
        updateFrame(srcXFrames[frame], srcYIdleFrames[movingDirection]);
    }

    if (movingDirections > 1)
        movementSpeed = 1.8;
    else
        movementSpeed = 3;

    float prev_desX = desX;
    float prev_desY = desY;

    moveCharacter();

    if (checkCollision(getLegFrame(), ObstaclesLower) || /*checkCollision(getChestFrame(), ObstaclesUpper) ||*/ outOfMap())
    {
            setDesX(prev_desX);
            setDesY(prev_desY);
    }
}
